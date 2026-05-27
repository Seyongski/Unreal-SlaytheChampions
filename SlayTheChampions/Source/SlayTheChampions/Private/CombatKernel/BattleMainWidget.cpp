#include "CombatKernel/BattleMainWidget.h"
#include "CombatKernel/CombatManager.h"
#include "CombatKernel/HandWidget.h"
#include "Unit/Unit.h"
#include "Card/CardUserComponent.h"
#include "Card/CardSubsystem.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"

// 위젯 초기화: CombatManager 탐색 및 바인딩, 플레이어 클릭 이벤트 바인딩, 마우스 활성화
void UBattleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// CombatManager 자동 탐색
	CombatManager = Cast<ACombatManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACombatManager::StaticClass()));

	if (CombatManager)
	{
		CombatManager->OnPhaseChanged.AddDynamic(this, &UBattleMainWidget::OnPhaseChanged);
		// BeginPlay에서 이미 StartTurn이 호출됐으므로 초기값 직접 설정
		if (Text_TurnCount)
			Text_TurnCount->SetText(FText::FromString(FString::Printf(TEXT("Turn %d"), CombatManager->TurnCount)));
		if (Text_Cost)
			Text_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), SharedCost, MaxCost)));

		// SpawnedPlayers 클릭 이벤트 바인딩
		BindPlayerClickEvents();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] CombatManager not found in level."));

	// HandPanel 카드 선택 이벤트 바인딩
	if (HandPanel)
		HandPanel->OnCardSelected.AddDynamic(this, &UBattleMainWidget::HandleCardClicked);

	// 버튼 바인딩 및 초기 가시성 설정
	if (Btn_EndTurn)
		Btn_EndTurn->OnClicked.AddDynamic(this, &UBattleMainWidget::HandleEndTurnClicked);

	if (Btn_NextPlayer)
	{
		Btn_NextPlayer->OnClicked.AddDynamic(this, &UBattleMainWidget::HandleNextPlayerClicked);
		// 플레이어 수가 1명이면 숨김, 2명 이상이면 핸드 선택 후 표시 (초기엔 숨김)
		Btn_NextPlayer->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (Btn_Cancel)
	{
		Btn_Cancel->OnClicked.AddDynamic(this, &UBattleMainWidget::HandleCancelClicked);
		Btn_Cancel->SetVisibility(ESlateVisibility::Collapsed);
	}

	// SpawnedEnemies 클릭 이벤트 바인딩
	BindEnemyClickEvents();

	// 마우스 커서 표시
	UMouseManager* MouseManager = GetGameInstance()->GetSubsystem<UMouseManager>();
	if (MouseManager)
		MouseManager->SetMouseVisibility(GetOwningPlayer(), true);

	// 평소엔 클릭 통과 — 카드 대기 상태 진입 시에만 Visible로 전환
	if (MainCanvas)
		MainCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

// 페이즈 전환 시 호출
// DrawPhase → 턴 카운트 텍스트 갱신, PlayerActionPhase → 코스트 리셋
void UBattleMainWidget::OnPhaseChanged(ETurnPhase NewPhase)
{
	if (!CombatManager) return;

	if (NewPhase == ETurnPhase::DrawPhase)
	{
		// 새 턴 시작 시 턴 카운트 텍스트 갱신
		if (Text_TurnCount)
			Text_TurnCount->SetText(FText::FromString(FString::Printf(TEXT("Turn %d"), CombatManager->TurnCount)));
	}
	else if (NewPhase == ETurnPhase::PlayerActionPhase)
	{
		// 플레이어 행동 턴 진입 시 공유 코스트 풀로 리셋
		SharedCost = MaxCost;
		if (Text_Cost)
			Text_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), SharedCost, MaxCost)));
		OnCostChanged(SharedCost, MaxCost);
	}
}

// SpawnedPlayers 각각의 OnUnitClicked에 바인딩
void UBattleMainWidget::BindPlayerClickEvents()
{
	if (!CombatManager) return;

	for (AUnit* Unit : CombatManager->GetSpawnedPlayers())
	{
		if (Unit)
			Unit->OnUnitClicked.AddDynamic(this, &UBattleMainWidget::HandlePlayerClicked);
	}
}

// 플레이어 유닛 클릭 시 호출
// 이전 선택의 CardUserComponent 바인딩을 해제하고, 새 유닛의 CardUserComponent를 바인딩 후 현재 손패를 표시
void UBattleMainWidget::HandlePlayerClicked(AUnit* Unit)
{
	if (!Unit) return;
	UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Player clicked: %s"), *Unit->GetName());

	// SingleAlly 카드 대기 중: 클릭한 플레이어 유닛을 타겟으로 큐에 등록
	if (!PendingCardName.IsNone() && PendingCardData.TargetType == ETargetType::SingleAlly)
	{
		QueueCardAction(PendingCardData, Unit, PendingCardName);
		return;
	}

	// 그 외 대기 상태(SingleEnemy 등)이면 취소 후 플레이어 전환
	if (!PendingCardName.IsNone())
	{
		PendingCardName = NAME_None;
		// 대기 해제 → 캔버스 클릭 통과 복귀
		if (MainCanvas)
			MainCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		OnPendingCleared();
	}

	// 이전 선택 유닛의 CardUserComponent 바인딩 해제
	if (SelectedUnit)
	{
		UCardUserComponent* PrevCard = SelectedUnit->FindComponentByClass<UCardUserComponent>();
		if (PrevCard)
			PrevCard->OnHandChanged.RemoveDynamic(this, &UBattleMainWidget::HandleHandChanged);
	}

	SelectedUnit = Unit;

	// 플레이어가 2명 이상이면 다음 플레이어 버튼 표시
	if (Btn_NextPlayer && CombatManager && CombatManager->GetSpawnedPlayers().Num() > 1)
		Btn_NextPlayer->SetVisibility(ESlateVisibility::Visible);

	OnPlayerSelected(Unit);

	// 새 유닛의 CardUserComponent 바인딩 및 현재 손패 즉시 표시
	UCardUserComponent* CardComp = Unit->FindComponentByClass<UCardUserComponent>();
	if (CardComp)
	{
		CardComp->OnHandChanged.AddDynamic(this, &UBattleMainWidget::HandleHandChanged);
		HandleHandChanged(CardComp->GetHand());
	}
	else
	{
		// CardUserComponent가 없으면 빈 손패 전달
		OnHandUpdated(TArray<FCardDataRow>());
	}
}

// CardUserComponent::OnHandChanged 수신
// 카드 ID 목록을 CardSubsystem으로 조회해 FCardDataRow 배열로 변환 후 BP에 전달
void UBattleMainWidget::HandleHandChanged(const TArray<FName>& CardNames)
{
	UCardSubsystem* CS = GetGameInstance()
		? GetGameInstance()->GetSubsystem<UCardSubsystem>()
		: nullptr;

	TArray<FCardDataRow> Cards;

	if (CS)
	{
		for (const FName& Name : CardNames)
		{
			const FCardDataRow* Row = CS->GetCard(Name);
			if (Row) Cards.Add(*Row);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("[BattleMainWidget] Hand updated: %d cards"), Cards.Num());
	for (const FCardDataRow& Card : Cards)
		UE_LOG(LogTemp, Log, TEXT("[BattleMainWidget]  - %s"), *Card.CardID.ToString());

	// HandPanel이 연결되어 있으면 C++로 직접 ShowHand 호출, 없으면 BP 이벤트로 폴백
	if (HandPanel)
		HandPanel->ShowHand(Cards);
	else
		OnHandUpdated(Cards);
}

// HandPanel::OnCardSelected 수신
// WBP_Card가 broadcast하는 CardID 필드값을 Row Name으로 변환 후 코스트 검증 및 큐 등록
void UBattleMainWidget::HandleCardClicked(FName CardName)
{
	if (!SelectedUnit || !CombatManager) return;

	UCardSubsystem* CS = GetGameInstance()
		? GetGameInstance()->GetSubsystem<UCardSubsystem>()
		: nullptr;
	if (!CS) return;

	// CardName은 WBP_Card의 CardID 필드값 — Row Name과 다를 수 있으므로 역조회
	FName RowName = CS->GetRowNameByCardID(CardName);
	if (RowName.IsNone()) RowName = CardName; // Row Name == CardID인 경우 폴백

	// 같은 카드 재선택 시 선택 해제 (Row Name 기준 비교)
	if (PendingCardName == RowName)
	{
		PendingCardName = NAME_None;
		// 대기 해제 → 캔버스 클릭 통과 복귀
		if (MainCanvas)
			MainCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		OnPendingCleared();
		return;
	}

	const FCardDataRow* Row = CS->GetCard(RowName);
	if (!Row) return;

	// 코스트 부족 시 무시
	if (SharedCost < Row->Cost)
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Not enough cost for %s (need %d, have %d)"),
			*CardName.ToString(), Row->Cost, SharedCost);
		return;
	}

	// Self / AllAllies / AllEnemies 타겟은 적 선택 없이 즉시 큐에 등록
	if (Row->TargetType == ETargetType::Self       ||
		Row->TargetType == ETargetType::AllAllies  ||
		Row->TargetType == ETargetType::AllEnemies ||
		Row->TargetType == ETargetType::Single_Team)
	{
		QueueCardAction(*Row, nullptr, RowName);
		return;
	}

	// SingleEnemy / SingleAlly: 적(아군) 선택 대기 상태로 전환 — Row Name 저장
	PendingCardName = RowName;
	PendingCardData = *Row;
	// 대기 진입 → 캔버스를 Visible로 전환해 NativeOnMouseButtonDown이 빈 영역 클릭을 수신하도록 함
	if (MainCanvas)
		MainCanvas->SetVisibility(ESlateVisibility::Visible);
	OnCardPending(CardName); // UI에는 CardID 전달 (표시용)
	UE_LOG(LogTemp, Log, TEXT("[BattleMainWidget] Card pending: %s (RowName: %s) — waiting for target"),
		*CardName.ToString(), *RowName.ToString());
}

// 적 유닛 클릭 시 호출
// 클릭 수신 여부를 항상 로그로 출력, PendingCard가 있으면 타겟으로 큐에 등록
void UBattleMainWidget::HandleEnemyClicked(AUnit* Enemy)
{
	if (!Enemy) return;

	// 적 클릭 수신 확인 로그 (PendingCard 여부와 무관하게 항상 출력)
	UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Enemy clicked: %s | PendingCard: %s"),
		*Enemy->GetName(),
		PendingCardName.IsNone() ? TEXT("none") : *PendingCardName.ToString());

	if (PendingCardName.IsNone()) return;

	// PendingCardName은 Row Name으로 저장되어 있음 — QueueCardAction에 함께 전달
	QueueCardAction(PendingCardData, Enemy, PendingCardName);
}

// SpawnedEnemies 각각의 OnUnitClicked에 바인딩
void UBattleMainWidget::BindEnemyClickEvents()
{
	if (!CombatManager) return;

	int32 BoundCount = 0;
	for (AUnit* Unit : CombatManager->GetSpawnedEnemies())
	{
		if (Unit)
		{
			Unit->OnUnitClicked.AddDynamic(this, &UBattleMainWidget::HandleEnemyClicked);
			BoundCount++;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Enemy click events bound: %d enemies"), BoundCount);
}

// 카드+타겟이 확정됐을 때 호출
// Hand에서 카드를 제거하고 CombatManager 큐에 등록한 뒤 코스트를 차감
// CardRowName — RemoveFromHand와 DiscardSpecificCard에 사용할 Row Name (없으면 CardID 폴백)
void UBattleMainWidget::QueueCardAction(const FCardDataRow& CardData, AUnit* TargetOverride, FName CardRowName)
{
	if (!SelectedUnit || !CombatManager) return;

	const int32 CasterIndex = CombatManager->GetSpawnedPlayers().IndexOfByKey(SelectedUnit);
	if (CasterIndex == INDEX_NONE) return;

	UCardUserComponent* CardComp = SelectedUnit->FindComponentByClass<UCardUserComponent>();
	if (!CardComp) return;

	// RemoveFromHand에 Row Name 우선 사용 (CardID != Row Name인 DataTable 구성 대응)
	const FName RemoveKey = CardRowName.IsNone() ? CardData.CardID : CardRowName;

	// Hand에서만 카드 제거 + OnHandChanged 브로드캐스트 → 손패 UI 즉시 갱신
	// DiscardPile 이동은 PlayerExecutionPhase에서 ExecuteNextAction이 처리
	if (CardComp->RemoveFromHand(RemoveKey))
	{
		UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Card removed from hand: %s | Hand count: %d"),
			*RemoveKey.ToString(), CardComp->GetHandCount());

		SharedCost -= CardData.Cost;
		if (Text_Cost)
			Text_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), SharedCost, MaxCost)));
		OnCostChanged(SharedCost, MaxCost);

		// PlayerActionPhase 큐에 등록 — PlayerExecutionPhase에서 실행됨
		CombatManager->QueuePlayerAction(CardData, CasterIndex, CardRowName, TargetOverride);

		UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Card queued: %s | Cost left: %d"),
			*CardData.CardID.ToString(), SharedCost);

		// 큐에 카드가 생겼으므로 취소 버튼 표시
		if (Btn_Cancel)
			Btn_Cancel->SetVisibility(ESlateVisibility::Visible);
	}

	// 선택 대기 상태 초기화 및 캔버스 클릭 통과 복귀
	PendingCardName = NAME_None;
	if (MainCanvas)
		MainCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnPendingCleared();
}

// Btn_EndTurn 클릭 → PlayerExecutionPhase 진입 후 큐 실행
void UBattleMainWidget::HandleEndTurnClicked()
{
	if (CombatManager)
		CombatManager->EndPlayerActionPhase();
}

// Btn_NextPlayer 클릭 → 다음 살아있는 플레이어로 선택 전환
void UBattleMainWidget::HandleNextPlayerClicked()
{
	SelectNextPlayer();
}

// Btn_Cancel 클릭 → 마지막 큐 액션을 취소하고 해당 플레이어 손패로 카드 반환
void UBattleMainWidget::HandleCancelClicked()
{
	if (!CombatManager) return;

	FQueuedAction LastAction;
	if (!CombatManager->PopLastPlayerAction(LastAction)) return;

	// 카드를 사용한 플레이어의 CardUserComponent에 카드 반환
	const TArray<AUnit*>& Players = CombatManager->GetSpawnedPlayers();
	if (Players.IsValidIndex(LastAction.CasterIndex) && Players[LastAction.CasterIndex])
	{
		UCardUserComponent* CardComp = Players[LastAction.CasterIndex]->FindComponentByClass<UCardUserComponent>();
		if (CardComp)
		{
			const FName ReturnKey = LastAction.CardRowName.IsNone() ? LastAction.Card.CardID : LastAction.CardRowName;
			CardComp->AddToHand(ReturnKey);
		}
	}

	// 코스트 환불 및 UI 갱신
	SharedCost += LastAction.Card.Cost;
	if (Text_Cost)
		Text_Cost->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), SharedCost, MaxCost)));
	OnCostChanged(SharedCost, MaxCost);

	UE_LOG(LogTemp, Warning, TEXT("[BattleMainWidget] Cancelled: %s | Cost restored: %d"),
		*LastAction.Card.CardID.ToString(), SharedCost);

	// 큐가 비었으면 취소 버튼 숨김
	if (Btn_Cancel && CombatManager->GetActionQueueCount() == 0)
		Btn_Cancel->SetVisibility(ESlateVisibility::Collapsed);
}

// 타겟 대기 중 빈 영역 또는 유효하지 않은 위치 클릭 시 대기 카드 취소
// 유닛이 클릭된 경우에는 기존 HandleEnemyClicked / HandlePlayerClicked 흐름에 위임
FReply UBattleMainWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 대기 카드 없거나 좌클릭이 아니면 기본 처리
	if (PendingCardName.IsNone() || InMouseEvent.GetEffectingButton() != EKeys::LeftMouseButton)
		return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 클릭 위치에 살아있는 유닛이 있으면 NotifyActorOnClicked 흐름(HandleEnemyClicked 등)에 위임
	APlayerController* PC = GetOwningPlayer();
	if (PC)
	{
		FHitResult Hit;
		if (PC->GetHitResultUnderCursor(ECC_Visibility, false, Hit))
		{
			AUnit* HitUnit = Cast<AUnit>(Hit.GetActor());
			if (HitUnit && HitUnit->IsAlive())
				return FReply::Unhandled();
		}
	}

	// 유효한 유닛 없는 곳 클릭 → 대기 취소
	CancelPendingCard();
	return FReply::Unhandled();
}

// 현재 대기 중인 카드 선택을 취소하고 BP에 알림
void UBattleMainWidget::CancelPendingCard()
{
	if (PendingCardName.IsNone()) return;
	PendingCardName = NAME_None;
	// 대기 해제 → 캔버스 클릭 통과 복귀
	if (MainCanvas)
		MainCanvas->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	OnPendingCleared();
}

// SpawnedPlayers에서 현재 선택 다음 인덱스의 살아있는 플레이어로 전환
void UBattleMainWidget::SelectNextPlayer()
{
	if (!CombatManager) return;

	const TArray<AUnit*>& Players = CombatManager->GetSpawnedPlayers();
	if (Players.Num() <= 1) return;

	const int32 CurrentIndex = Players.IndexOfByKey(SelectedUnit);
	// 현재 인덱스 다음부터 순환하며 살아있는 플레이어 탐색
	for (int32 i = 1; i < Players.Num(); i++)
	{
		const int32 NextIndex = (CurrentIndex + i) % Players.Num();
		if (Players.IsValidIndex(NextIndex) && Players[NextIndex] && Players[NextIndex]->IsAlive())
		{
			HandlePlayerClicked(Players[NextIndex]);
			return;
		}
	}
}
