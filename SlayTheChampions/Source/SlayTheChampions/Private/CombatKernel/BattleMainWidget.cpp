// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatKernel/BattleMainWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "CombatKernel/CardWidget.h"
#include "CombatKernel/CardPlayerController.h"
#include "CombatKernel/WidgetCardsStruct.h"

void UBattleMainWidget::RefreshStatsUI()
{
	if (Party0HPText)
		Party0HPText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Party0_HP, Party0_MaxHP)));
	if (Party0BlockText)
		Party0BlockText->SetText(FText::FromString(FString::Printf(TEXT("방어도: %d"), Party0_Block)));

	if (Party1HPText)
		Party1HPText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), Party1_HP, Party1_MaxHP)));
	if (Party1BlockText)
		Party1BlockText->SetText(FText::FromString(FString::Printf(TEXT("방어도: %d"), Party1_Block)));

	if (EnemyHPText)
		EnemyHPText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), EnemyHP, EnemyMaxHP)));
}

void UBattleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (AddNewCardButton)
	{
		AddNewCardButton->OnClicked.AddDynamic(this, &UBattleMainWidget::API_AddNewPlayerCard);
	}

	RefreshStatsUI();
}

void UBattleMainWidget::API_AddNewPlayerCard()
{
	// 테스트용 카드 3종 순환 주입 (Attack → Defense → Heal)
	static int32 TestCardIndex = 0;

	FCardStruct TestCard;
	switch (TestCardIndex % 3)
	{
	case 0:
		TestCard.Name        = FName("TestAttack");
		TestCard.CardName    = FText::FromString(TEXT("테스트 공격"));
		TestCard.Description = FText::FromString(TEXT("적에게 6 피해"));
		TestCard.CardType    = ECardType::Attack;
		TestCard.TargetType  = ETargetType::SingleEnemy;
		TestCard.Cost        = 1;
		TestCard.Damage      = 6;
		break;
	case 1:
		TestCard.Name        = FName("TestDefense");
		TestCard.CardName    = FText::FromString(TEXT("테스트 방어"));
		TestCard.Description = FText::FromString(TEXT("방어도 5 획득"));
		TestCard.CardType    = ECardType::Defense;
		TestCard.TargetType  = ETargetType::Self;
		TestCard.Cost        = 1;
		TestCard.Block       = 5;
		break;
	case 2:
		TestCard.Name        = FName("TestHeal");
		TestCard.CardName    = FText::FromString(TEXT("테스트 회복"));
		TestCard.Description = FText::FromString(TEXT("아군 1명을 4 회복"));
		TestCard.CardType    = ECardType::Buff;
		TestCard.TargetType  = ETargetType::SingleAlly;
		TestCard.Cost        = 1;
		TestCard.HealAmount  = 4;
		break;
	}
	TestCardIndex++;

	AddCard(TestCard);

	if (!bDoOnce)
	{
		bDoOnce = true;
		GetWorld()->GetTimerManager().SetTimer(
			OrganizeTimerHandle,
			this,
			&UBattleMainWidget::OnOrganizeDelay,
			0.5f,
			false
		);
	}
}

void UBattleMainWidget::OnOrganizeDelay()
{
	OrganizeCards(20.0f);
	bDoOnce = false;
}

void UBattleMainWidget::AddCard(const FCardStruct& InCardData)
{
	UCardWidget* LocalWidgetCard = CreateWidget<UCardWidget>(GetOwningPlayer(), NewCard);

	if (LocalWidgetCard && MainCanvas)
	{
		LocalWidgetCard->SetCardData(InCardData);
		LocalWidgetCard->OnCardClicked.AddDynamic(this, &UBattleMainWidget::HandleCardClicked);

		UCanvasPanelSlot* CanvasSlot = MainCanvas->AddChildToCanvas(LocalWidgetCard);
		if (CanvasSlot)
		{
			CanvasSlot->SetAutoSize(true);
		}

		FWidgetCardsStruct NewEntry;
		NewEntry.CardWidget = LocalWidgetCard;
		NewEntry.Canvas = MainCanvas;
		NewEntry.CardSlot = CanvasSlot;

		WidgetCards.Add(NewEntry);
	}
}

void UBattleMainWidget::SelectPartyMember(int32 Index)
{
	if (Index < 0 || Index > 1) return;
	ActivePartyIndex = Index;
}

void UBattleMainWidget::OnCardExecuted_Implementation(const FCardStruct& Card)
{
	switch (Card.CardType)
	{
	case ECardType::Attack:
	case ECardType::AOE:
		EnemyHP = FMath::Max(0, EnemyHP - Card.Damage);
		OnEnemyStatsChanged(EnemyHP, EnemyMaxHP);
		break;

	case ECardType::Defense:
		if (ActivePartyIndex == 0)
		{
			Party0_Block += Card.Block;
			OnPartyStatsChanged(0, Party0_HP, Party0_MaxHP, Party0_Block);
		}
		else
		{
			Party1_Block += Card.Block;
			OnPartyStatsChanged(1, Party1_HP, Party1_MaxHP, Party1_Block);
		}
		break;

	case ECardType::Buff:
		if (ActivePartyIndex == 0)
		{
			Party0_HP = FMath::Min(Party0_MaxHP, Party0_HP + Card.HealAmount);
			OnPartyStatsChanged(0, Party0_HP, Party0_MaxHP, Party0_Block);
		}
		else
		{
			Party1_HP = FMath::Min(Party1_MaxHP, Party1_HP + Card.HealAmount);
			OnPartyStatsChanged(1, Party1_HP, Party1_MaxHP, Party1_Block);
		}
		break;

	default:
		break;
	}

	RefreshStatsUI();
}

void UBattleMainWidget::HandleCardClicked(UCardWidget* Widget, const FCardStruct& Card)
{
	// 코스트 부족 시 무시
	if (SharedCost < Card.Cost) return;

	// 손패에서 해당 위젯을 찾아 제거
	const int32 RemoveIndex = WidgetCards.IndexOfByPredicate([&](const FWidgetCardsStruct& Entry)
	{
		return Entry.CardWidget == Widget;
	});

	if (RemoveIndex != INDEX_NONE)
	{
		if (WidgetCards[RemoveIndex].CardWidget)
		{
			WidgetCards[RemoveIndex].CardWidget->RemoveFromParent();
		}
		WidgetCards.RemoveAt(RemoveIndex);
	}

	// 코스트 차감
	SharedCost -= Card.Cost;
	OnCostChanged(SharedCost, MaxCost);

	// 카드 효과 실행 이벤트 (BP에서 데미지/방어 처리)
	OnCardExecuted(Card);

	OrganizeCards(20.0f);
}

void UBattleMainWidget::OrganizeCards(float OffsetX)
{
	float OffsetCardX = OffsetX;
	int32 CardCount = 0;

	float OutBottomPadding = 0.f;
	float OutCenterOfScreen = 0.f;
	FN_GetScreenInfo(BottomMargin, CardHeight, OutBottomPadding, OutCenterOfScreen);

	UE_LOG(LogTemp, Warning, TEXT("[OrganizeCards] CardPlayerController: %s | BottomPadding: %.1f | CenterOfScreen: %.1f"),
		CardPlayerController ? TEXT("유효") : TEXT("NULL"),
		OutBottomPadding, OutCenterOfScreen);

	for (int32 i = 0; i < WidgetCards.Num(); i++)
	{
		CardCount++;
		int32 CardCurrentIndex = i;

		UCanvasPanelSlot* CardSlot = WidgetCards[i].CardSlot;

		float PositionX = OutBottomPadding + (OffsetCardX * CardCurrentIndex);
		float PositionY = OutCenterOfScreen;

		if (CardSlot)
		{
			CardSlot->SetAutoSize(false);
			CardSlot->SetSize(FVector2D(CardWidth, CardHeight));
			CardSlot->SetPosition(FVector2D(PositionX, PositionY));
			CardSlot->SetZOrder(CardCount);
		}
	}
}

void UBattleMainWidget::FN_GetScreenInfo(float InBottomMargin, float InCardHeight, float& OutBottomPadding, float& OutCenterOfScreen)
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);

	float ViewportX = ViewportSize.X;
	float ViewportY = ViewportSize.Y;

	OutCenterOfScreen = ViewportX / 3.0f;
	OutBottomPadding = (ViewportY - InBottomMargin) - (InCardHeight * 1.5f);
}
