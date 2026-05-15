// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatTypes.generated.h"

class AUnit;

//진영 구분
UENUM(BlueprintType)
enum class ETeam : uint8
{
    Ally    UMETA(DisplayName = "Ally"),
    Enemy   UMETA(DisplayName = "Enemy"),
    Neutral UMETA(DisplayName = "Neutral")
};

//UCLASS()
//class SLAYTHECHAMPIONS_API ACombatTypes : public AActor
//{
//	GENERATED_BODY()
//	
//
//};
