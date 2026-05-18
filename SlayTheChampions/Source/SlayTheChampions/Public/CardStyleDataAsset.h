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
    // 이 스타일이 어떤 직업용인지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle")
    EJobClass JobClass = EJobClass::Any;

    // 희귀도별 테두리 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Border")
    TSoftObjectPtr<UTexture2D> BorderImage_Normal;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Border")
    TSoftObjectPtr<UTexture2D> BorderImage_Rare;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Border")
    TSoftObjectPtr<UTexture2D> BorderImage_Legendary;

    // 희귀도별 보석 이미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Gem")
    TSoftObjectPtr<UTexture2D> GemImage_Normal;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Gem")
    TSoftObjectPtr<UTexture2D> GemImage_Rare;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CardStyle|Gem")
    TSoftObjectPtr<UTexture2D> GemImage_Legendary;

    // Rarity에 맞는 테두리 이미지 반환
    UFUNCTION(BlueprintCallable, Category = "CardStyle")
    UTexture2D* GetBorderImage(ECardRarity Rarity) const;

    // Rarity에 맞는 보석 이미지 반환
    UFUNCTION(BlueprintCallable, Category = "CardStyle")
    UTexture2D* GetGemImage(ECardRarity Rarity) const;
};