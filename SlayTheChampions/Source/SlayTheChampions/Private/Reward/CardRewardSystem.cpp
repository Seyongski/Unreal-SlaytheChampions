#include "Reward/CardRewardSystem.h"

#include "Card/CardSaveGame.h"
#include "Card/CardSubsystem.h"
#include "Party/PartyInstance.h"

void UCardRewardSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

TArray<FName> UCardRewardSystem::GetRewardCardChoices(int32 PawnIndex) const
{
    TArray<FName> Choices;
    UCardSubsystem* LocalCardSubsystem = GetCardSubsystem();
    if (CardRewardChoiceCount <= 0 || !LocalCardSubsystem)
    {
        return Choices;
    }

    const EJobClass JobClass = ResolveRewardJobClass(PawnIndex);
    Choices.Reserve(CardRewardChoiceCount);

    while (Choices.Num() < CardRewardChoiceCount)
    {
        const ECardRarity RolledRarity = RollRewardCardRarity();
        TArray<FName> RewardPool = GetRewardPoolByRarity(JobClass, RolledRarity);
        RewardPool.RemoveAll([&Choices](const FName& CardId)
        {
            return Choices.Contains(CardId);
        });

        if (RewardPool.IsEmpty())
        {
            RewardPool = GetRewardPoolByRarity(JobClass, ECardRarity::Normal);
            RewardPool.RemoveAll([&Choices](const FName& CardId)
            {
                return Choices.Contains(CardId);
            });
        }

        if (RewardPool.IsEmpty())
        {
            break;
        }

        const int32 PickIndex = FMath::RandRange(0, RewardPool.Num() - 1);
        Choices.Add(RewardPool[PickIndex]);
    }

    return Choices;
}

bool UCardRewardSystem::GetCardDataById(FName CardID, FCardDataRow& OutCardData) const
{
    UCardSubsystem* LocalCardSubsystem = GetCardSubsystem();
    if (!LocalCardSubsystem)
    {
        return false;
    }

    const FCardDataRow* CardData = LocalCardSubsystem->GetCard(CardID);
    if (!CardData)
    {
        return false;
    }

    OutCardData = *CardData;
    return true;
}

EJobClass UCardRewardSystem::GetRewardJobClass(int32 PawnIndex) const
{
    return ResolveRewardJobClass(PawnIndex);
}

FText UCardRewardSystem::GetRewardJobName(int32 PawnIndex) const
{
    const EJobClass JobClass = ResolveRewardJobClass(PawnIndex);
    const UEnum* JobEnum = StaticEnum<EJobClass>();
    if (!JobEnum)
    {
        return FText::FromString(TEXT("Unknown"));
    }

    return JobEnum->GetDisplayNameTextByValue(static_cast<int64>(JobClass));
}

int32 UCardRewardSystem::GetPartyRewardTargetCount() const
{
    UPartyInstance* LocalPartyInstance = GetPartyInstance();
    if (!LocalPartyInstance)
    {
        return 1;
    }

    const int32 PartyCount = LocalPartyInstance->GetPartyMemberCount();
    return PartyCount > 0 ? PartyCount : 1;
}

void UCardRewardSystem::SetCardRewardChoiceCount(int32 InCount)
{
    CardRewardChoiceCount = FMath::Max(1, InCount);
}

EJobClass UCardRewardSystem::ResolveRewardJobClass(int32 PawnIndex) const
{
    const EJobClass SavedJobClass = UCardSaveGame::GetJobClass(PawnIndex);
    if (SavedJobClass != EJobClass::Any)
    {
        return SavedJobClass;
    }

    switch (PawnIndex)
    {
    case 0:
        return EJobClass::Warrior;
    case 1:
        return EJobClass::Mage;
    default:
        return EJobClass::Any;
    }
}

TArray<FName> UCardRewardSystem::GetRewardPoolByRarity(EJobClass JobClass, ECardRarity CardRarity) const
{
    TArray<FName> RewardPool;
    UCardSubsystem* LocalCardSubsystem = GetCardSubsystem();
    if (!LocalCardSubsystem)
    {
        return RewardPool;
    }

    const TArray<FName> AllCards = LocalCardSubsystem->GetRewardPool(JobClass, ECardRarity::Normal);
    for (const FName& CardId : AllCards)
    {
        const FCardDataRow* CardData = LocalCardSubsystem->GetCard(CardId);
        if (!CardData)
        {
            continue;
        }

        if (CardData->Rarity == CardRarity)
        {
            RewardPool.Add(CardId);
        }
    }

    return RewardPool;
}

UCardSubsystem* UCardRewardSystem::GetCardSubsystem() const
{
    if (CardSubsystem)
    {
        return CardSubsystem;
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UCardRewardSystem* MutableThis = const_cast<UCardRewardSystem*>(this);
        MutableThis->CardSubsystem = GameInstance->GetSubsystem<UCardSubsystem>();
        return MutableThis->CardSubsystem;
    }

    return nullptr;
}

UPartyInstance* UCardRewardSystem::GetPartyInstance() const
{
    if (PartyInstance)
    {
        return PartyInstance;
    }

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        UCardRewardSystem* MutableThis = const_cast<UCardRewardSystem*>(this);
        MutableThis->PartyInstance = GameInstance->GetSubsystem<UPartyInstance>();
        return MutableThis->PartyInstance;
    }

    return nullptr;
}

ECardRarity UCardRewardSystem::RollRewardCardRarity() const
{
    const int32 Roll = FMath::RandRange(1, 100);
    const int32 NormalChance = static_cast<int32>(ECardChance::Nomal_Card);
    const int32 RareChance = static_cast<int32>(ECardChance::Rare_Card);

    if (Roll <= NormalChance)
    {
        return ECardRarity::Normal;
    }

    if (Roll <= NormalChance + RareChance)
    {
        return ECardRarity::Rare;
    }

    return ECardRarity::Legendary;
}
