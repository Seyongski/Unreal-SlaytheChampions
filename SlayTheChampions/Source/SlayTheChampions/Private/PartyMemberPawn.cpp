#include "PartyMemberPawn.h"
#include "HandComponent.h"
#include "CardSubsystem.h"

APartyMemberPawn::APartyMemberPawn()
{
    PrimaryActorTick.bCanEverTick = false;
    HandComponent = CreateDefaultSubobject<UHandComponent>(TEXT("HandComponent"));
}

void APartyMemberPawn::BeginPlay()
{
    Super::BeginPlay();
    InitializeDeck();
}

void APartyMemberPawn::InitializeDeck()
{
    if (!HandComponent) return;

    TArray<FName> DeckNames;

    if (OverrideDeckNames.Num() > 0)
    {
        DeckNames = OverrideDeckNames;
    }
    else
    {
        if (UGameInstance* GI = GetGameInstance())
        {
            if (UCardSubsystem* CS = GI->GetSubsystem<UCardSubsystem>())
            {
                DeckNames = CS->GetCardNamesByClass(JobClass);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("[%s] InitializeDeck: CardSubsystem 없음"), *GetName());
                return;
            }
        }
    }

    if (DeckNames.IsEmpty())
    {
        const int32 JobClassInt = static_cast<int32>(JobClass);
        UE_LOG(LogTemp, Warning, TEXT("[%s] InitializeDeck: JobClass=%d 카드 없음"), *GetName(), JobClassInt);
        return;
    }

    HandComponent->InitializeDeck(DeckNames);

    // 덱 초기화 완료 로그
    const int32 DeckCount = DeckNames.Num();
    UE_LOG(LogTemp, Log, TEXT("[%s] 덱 초기화 완료 - 총 %d장"), *GetName(), DeckCount);
    for (const FName& CardName : DeckNames)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s]   DrawPile: %s"), *GetName(), *CardName.ToString());
    }
}

void APartyMemberPawn::DrawStartOfTurn()
{
    if (!HandComponent) return;

    HandComponent->DrawStartOfTurn();

    // 드로우 후 손패 로그
    const TArray<FName>& Hand = HandComponent->GetHand();
    UE_LOG(LogTemp, Log, TEXT("[%s] 손패 (%d장):"), *GetName(), Hand.Num());
    for (const FName& CardName : Hand)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s]   Hand: %s"), *GetName(), *CardName.ToString());
    }
}

bool APartyMemberPawn::PlayCard(FName CardName)
{
    if (!HandComponent) return false;

    const bool bResult = HandComponent->PlayCard(CardName);
    if (bResult)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] 카드 사용: %s | 남은 손패 %d장"),
            *GetName(), *CardName.ToString(), HandComponent->GetHandCount());
    }
    return bResult;
}

void APartyMemberPawn::DiscardHand()
{
    if (!HandComponent) return;
    HandComponent->DiscardHand();
    UE_LOG(LogTemp, Log, TEXT("[%s] 손패 버림 | Discard %d장"), *GetName(), HandComponent->GetDiscardPileCount());
}

TArray<FName> APartyMemberPawn::GetHand() const
{
    return HandComponent ? HandComponent->GetHand() : TArray<FName>();
}

int32 APartyMemberPawn::GetDrawPileCount() const
{
    return HandComponent ? HandComponent->GetDrawPileCount() : 0;
}

int32 APartyMemberPawn::GetDiscardPileCount() const
{
    return HandComponent ? HandComponent->GetDiscardPileCount() : 0;
}