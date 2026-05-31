#include "Reward/BattleClearWidget.h"
#include "Reward/CardRewardSystem.h"
#include "Relic/RelicSubsystem.h"
#include "Map/RunSystem.h"
#include "Reward/RewardStruct.h"

void UBattleClearWidget::NativeConstruct()
{
    Super::NativeConstruct();

    OnVisibilityChanged.AddUniqueDynamic(this, &UBattleClearWidget::HandleVisibilityChanged);
}

TArray<FRewardData> UBattleClearWidget::GenerateRewardData(EAreaType _type)
{
    TArray<FRewardData> Rewards;

    auto AddReward = [&Rewards](ERewardTypes RewardType, int32 Amount = 0, FName RewardId = NAME_None)
    {
        FRewardData RewardData;
        RewardData.RewardType = RewardType;
        RewardData.Amount = Amount;
        RewardData.RewardId = RewardId;
        Rewards.Add(RewardData);
    };

    const int32 CardChoiceCount = GetCardRewardChoiceCount();

    switch (_type)
    {
    case EAreaType::Normal:
        AddReward(ERewardTypes::NomalCard, CardChoiceCount, TEXT("NormalCardReward"));
        AddReward(ERewardTypes::Gold, RollGold(_type), TEXT("Gold"));

        if (RollChance(PotionRewardChances::Normal))
        {
            AddReward(ERewardTypes::Potion, 1, TEXT("Potion"));
        }
        break;

    case EAreaType::Elite:
        AddReward(ERewardTypes::NomalCard, CardChoiceCount, TEXT("NormalCardReward"));
        AddReward(ERewardTypes::Gold, RollGold(_type), TEXT("Gold"));
        if (UGameInstance* GameInstance = GetGameInstance())
        {
            if (URelicSubsystem* RelicSubsystem = GameInstance->GetSubsystem<URelicSubsystem>())
            {
                const FName RandomRelicId = RelicSubsystem->GetRandomCommonRelic();
                AddReward(ERewardTypes::Relic, 1, RandomRelicId);
            }
        }

        if (RollChance(PotionRewardChances::Elite))
        {
            AddReward(ERewardTypes::Potion, 1, TEXT("Potion"));
        }
        break;

    case EAreaType::Boss:
        AddReward(ERewardTypes::LegendCard, CardChoiceCount, TEXT("LegendCardReward"));
        AddReward(ERewardTypes::Gold, RollGold(_type), TEXT("Gold"));

        if (RollChance(PotionRewardChances::Boss))
        {
            AddReward(ERewardTypes::Potion, 1, TEXT("Potion"));
        }
        break;
        
    default:
        break;
    }

    return Rewards;
}

void UBattleClearWidget::RefreshRewards()
{
    EAreaType AreaType = EAreaType::Normal;

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (URunSystem* RunSystem = GameInstance->GetSubsystem<URunSystem>())
        {
            AreaType = RunSystem->GetCurrentRoomInfo().AreaType;
        }
    }

    CurrentRewardData = GenerateRewardData(AreaType);
    OnRewardDataRefreshed(CurrentRewardData);
}

TArray<FName> UBattleClearWidget::GetCardRewardChoices(int32 PawnIndex) const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UCardRewardSystem* CardRewardSystem = GameInstance->GetSubsystem<UCardRewardSystem>())
        {
            return CardRewardSystem->GetRewardCardChoices(PawnIndex);
        }
    }

    return {};
}

int32 UBattleClearWidget::GetPartyRewardTargetCount() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UCardRewardSystem* CardRewardSystem = GameInstance->GetSubsystem<UCardRewardSystem>())
        {
            return CardRewardSystem->GetPartyRewardTargetCount();
        }
    }

    return 1;
}

void UBattleClearWidget::SetCardRewardChoiceCount(int32 InCount)
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UCardRewardSystem* CardRewardSystem = GameInstance->GetSubsystem<UCardRewardSystem>())
        {
            CardRewardSystem->SetCardRewardChoiceCount(InCount);
        }
    }
}

int32 UBattleClearWidget::GetCardRewardChoiceCount() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UCardRewardSystem* CardRewardSystem = GameInstance->GetSubsystem<UCardRewardSystem>())
        {
            return CardRewardSystem->GetCardRewardChoiceCount();
        }
    }

    return 3;
}

void UBattleClearWidget::HandleVisibilityChanged(ESlateVisibility InVisibility)
{
    if (InVisibility == ESlateVisibility::Visible ||
        InVisibility == ESlateVisibility::HitTestInvisible ||
        InVisibility == ESlateVisibility::SelfHitTestInvisible)
    {
        RefreshRewards();
    }
}

bool UBattleClearWidget::RollChance(float _Probability)
{
    const float Rand = FMath::FRandRange(1.f, 100.f);
    return Rand <= _Probability;
}

int32 UBattleClearWidget::RollGold(EAreaType _type)
{
    int32 gold = 0;
    switch (_type)
    {
    case EAreaType::Normal:
        gold = FMath::RandRange((int32)EGoldAmount::Min_Gold, (int32)EGoldAmount::Nomal_Gold);
        break;

    case EAreaType::Elite:
        gold = FMath::RandRange((int32)EGoldAmount::Nomal_Gold, (int32)EGoldAmount::Elite_Gold);
        break;

    case EAreaType::Boss:
        gold = FMath::RandRange((int32)EGoldAmount::Elite_Gold, (int32)EGoldAmount::Boss_Gold);
        break;
    }
    return gold;
}
