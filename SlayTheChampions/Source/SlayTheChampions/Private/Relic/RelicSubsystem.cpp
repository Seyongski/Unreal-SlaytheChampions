#include "Relic/RelicSubsystem.h"

#include "Engine/DataTable.h"
#include "UObject/EnumProperty.h"

void URelicSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	static const FSoftObjectPath RelicTablePath(TEXT("/Game/04_Data/RelicDataRow.RelicDataRow"));
	static const FSoftObjectPath RelicEffectTablePath(TEXT("/Game/04_Data/RelicEffectRow.RelicEffectRow"));

	if (UDataTable* LoadedRelicTable = Cast<UDataTable>(RelicTablePath.TryLoad()))
	{
		LoadRelicDataTable(LoadedRelicTable);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[RelicSubsystem] Failed to load relic data table: %s"), *RelicTablePath.ToString());
	}

	if (UDataTable* LoadedRelicEffectTable = Cast<UDataTable>(RelicEffectTablePath.TryLoad()))
	{
		LoadRelicEffectDataTable(LoadedRelicEffectTable);
	}

	InitializeRelics();
}

void URelicSubsystem::LoadRelicDataTable(UDataTable* InTable)
{
	RelicDataTable = InTable;
	InitializeRelics();
}

void URelicSubsystem::LoadRelicEffectDataTable(UDataTable* InTable)
{
	RelicEffectDataTable = InTable;
	InitializeRelics();
}

const FRelicDataRow* URelicSubsystem::GetRelicDataRow(FName InRelicID) const
{
	if (!RelicDataTable)
	{
		return nullptr;
	}

	for (const FName& RowName : RelicDataTable->GetRowNames())
	{
		const FRelicDataRow* Row = RelicDataTable->FindRow<FRelicDataRow>(RowName, TEXT("URelicSubsystem::GetRelicDataRow"));

		if (Row && Row->RelicID == InRelicID)
		{
			return Row;
		}
	}

	return nullptr;
}

TArray<FRelicEffectData> URelicSubsystem::GetRelicEffectData(FName InRelicID) const
{
	TArray<FRelicEffectData> Result;

	if (!RelicEffectDataTable)
	{
		return Result;
	}

	for (const FName& RowName : RelicEffectDataTable->GetRowNames())
	{
		const FRelicEffectRow* Row = RelicEffectDataTable->FindRow<FRelicEffectRow>(RowName, TEXT("URelicSubsystem::GetRelicEffectData"));

		if (!Row || Row->RelicID != InRelicID)
		{
			continue;
		}

		FRelicEffectData EffectData;
		EffectData.Order = Row->Order;
		EffectData.BuffType = Row->BuffType;
		EffectData.Value = Row->Value;
		EffectData.EffectiveDate = Row->EffectiveDate;
		EffectData.TriggerCondition = Row->TriggerCondition;
		EffectData.TriggerValue = Row->TriggerValue;
		EffectData.TriggerUsageType = Row->TriggerUsageType;
		Result.Add(EffectData);
	}

	Result.Sort([](const FRelicEffectData& A, const FRelicEffectData& B)
	{
		return A.Order < B.Order;
	});

	return Result;
}

bool URelicSubsystem::GetRelicRuntimeData(FName InRelicID, FRelicRuntimeData& OutRelicData) const
{
	const FRelicDataRow* RelicRow = GetRelicDataRow(InRelicID);

	if (!RelicRow)
	{
		return false;
	}

	OutRelicData.RelicID = RelicRow->RelicID;
	OutRelicData.RelicName = RelicRow->RelicName;
	OutRelicData.Description = RelicRow->Description;
	OutRelicData.Rarity = RelicRow->Rarity;
	OutRelicData.RelicSourceType = RelicRow->RelicSourceType;
	OutRelicData.TargetScope = RelicRow->TargetScope;
	OutRelicData.EffectiveDate = RelicRow->EffectiveDate;
	OutRelicData.Effects = GetRelicEffectData(InRelicID);
	return true;
}

TArray<FName> URelicSubsystem::GetAllRelicIDs() const
{
	TArray<FName> Result;

	if (!RelicDataTable)
	{
		return Result;
	}

	for (const FName& RowName : RelicDataTable->GetRowNames())
	{
		const FRelicDataRow* Row = RelicDataTable->FindRow<FRelicDataRow>(RowName, TEXT("URelicSubsystem::GetAllRelicIDs"));

		if (Row)
		{
			Result.Add(Row->RelicID);
		}
	}

	return Result;
}

bool URelicSubsystem::GetInitializedRelicData(FName InRelicID, FRelicRuntimeData& OutRelicData) const
{
	if (const FRelicRuntimeData* FoundRelic = InitializedRelicMap.Find(InRelicID))
	{
		OutRelicData = *FoundRelic;
		return true;
	}

	return false;
}

TArray<FRelicRuntimeData> URelicSubsystem::GetInitializedRelics() const
{
	return InitializedRelics;
}

void URelicSubsystem::LogInitializedRelics() const
{
	const UEnum* EffectTypeEnum = StaticEnum<EEffectType_test>();
	const UEnum* EffectiveDateEnum = StaticEnum<EEffectiveDate>();
	const UEnum* TriggerConditionEnum = StaticEnum<ERelicTriggerCondition>();
	const UEnum* TriggerUsageTypeEnum = StaticEnum<ERelicTriggerUsageType>();

	UE_LOG(LogTemp, Log, TEXT("[RelicSubsystem] Cached Relic Count=%d"), InitializedRelics.Num());

	for (const FRelicRuntimeData& RuntimeData : InitializedRelics)
	{
		UE_LOG(
			LogTemp,
			Log,
			TEXT("[RelicSubsystem] Cached Relic ID=%s Name=%s Effects=%d Description=%s"),
			*RuntimeData.RelicID.ToString(),
			*RuntimeData.RelicName.ToString(),
			RuntimeData.Effects.Num(),
			*RuntimeData.Description.ToString());

		for (const FRelicEffectData& EffectData : RuntimeData.Effects)
		{
			const FString EffectTypeText = EffectTypeEnum ? EffectTypeEnum->GetNameStringByValue(static_cast<int64>(EffectData.BuffType)) : TEXT("Unknown");
			const FString EffectiveDateText = EffectiveDateEnum ? EffectiveDateEnum->GetNameStringByValue(static_cast<int64>(EffectData.EffectiveDate)) : TEXT("Unknown");
			const FString TriggerConditionText = TriggerConditionEnum ? TriggerConditionEnum->GetNameStringByValue(static_cast<int64>(EffectData.TriggerCondition)) : TEXT("Unknown");
			const FString TriggerUsageTypeText = TriggerUsageTypeEnum ? TriggerUsageTypeEnum->GetNameStringByValue(static_cast<int64>(EffectData.TriggerUsageType)) : TEXT("Unknown");

			UE_LOG(
				LogTemp,
				Log,
				TEXT("[RelicSubsystem]  Cached Effect Order=%d Type=%s Value=%d EffectiveDate=%s TriggerCondition=%s TriggerValue=%d Usage=%s"),
				EffectData.Order,
				*EffectTypeText,
				EffectData.Value,
				*EffectiveDateText,
				*TriggerConditionText,
				EffectData.TriggerValue,
				*TriggerUsageTypeText);
		}
	}
}

void URelicSubsystem::InitializeRelics()
{
	InitializedRelics.Reset();
	InitializedRelicMap.Reset();

	if (!RelicDataTable || !RelicEffectDataTable)
	{
		return;
	}

	for (const FName& RelicID : GetAllRelicIDs())
	{
		FRelicRuntimeData RuntimeData;
		if (!GetRelicRuntimeData(RelicID, RuntimeData))
		{
			continue;
		}

		InitializedRelicMap.Add(RelicID, RuntimeData);
		InitializedRelics.Add(RuntimeData);
	}

	LogInitializedRelics();
}
