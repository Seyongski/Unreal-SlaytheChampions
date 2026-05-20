#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectManager.generated.h"

class AUnit;

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	Block UMETA(DisplayName = "Block"),
};

UCLASS()
class SLAYTHECHAMPIONS_API UEffectManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Effect")
	static void ApplyEffect(AUnit* Target, EEffectType Type, int32 Value);
};
