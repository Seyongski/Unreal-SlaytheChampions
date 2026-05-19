// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Champion/ChampionStruct.h"
#include "ChampionInstance.generated.h"

/**
 *
 */
class AUnit;
UCLASS()
class SLAYTHECHAMPIONS_API UChampionInstance : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadOnly, Category = "ChampionInstance", meta = (AllowPrivateAccess = "true"))
	FChampionsInfo ChampionsInfo;

	//유물 추가 필요
public:
};
