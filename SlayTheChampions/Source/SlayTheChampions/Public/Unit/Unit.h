// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CombatTypes.h"
#include "Unit.generated.h"


/**
 * 전투에 등장하는 모든 액터의 베이스 Pawn.
 * 자체적으로 어떤 컴포넌트도 들고 있지 않으며,
 * 블루프린트 또는 스폰 코드에서 필요한 컴포넌트를 부착해 능력을 부여한다.
 */

UCLASS()
class SLAYTHECHAMPIONS_API AUnit : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUnit();

	//유닛 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	FName UnitID;

	//유닛 소속
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Unit")
	ETeam Team = ETeam::Enemy;

protected:
	

	virtual void BeginPlay() override;

};
