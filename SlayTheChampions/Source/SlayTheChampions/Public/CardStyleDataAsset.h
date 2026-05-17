#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/Texture2D.h"
#include "CardDataTypes.h"
#include "CardStyleDataAsset.generated.h"

UCLASS(BlueprintType)
class SLAYTHECHAMPIONS_API UCardStyleDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle")
    EJobClass JobClass = EJobClass::Any;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle")
    TSoftObjectPtr<UTexture2D> BorderImage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle")
    TSoftObjectPtr<UTexture2D> GemImage;
};