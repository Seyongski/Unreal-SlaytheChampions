// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatManager.generated.h"

class UCombatStatComponent;

UCLASS()
class SLAYTHECHAMPIONS_API ACombatManager : public AActor
{
	GENERATED_BODY()

public:
	ACombatManager();

	// 씬에 배치된 플레이어 액터 (에디터에서 직접 연결, 최대 2명)
	UPROPERTY(EditAnywhere, Category = "Combat|Setup")
	TArray<AActor*> PlayerActors;

	// 씬에 배치된 적 액터 (에디터에서 직접 연결, 최대 3명)
	UPROPERTY(EditAnywhere, Category = "Combat|Setup")
	TArray<AActor*> EnemyActors;

	// BeginPlay에서 자동 호출 — Role 세팅 및 초기 데이터 주입
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void InitCombat();

	// 인덱스로 플레이어 StatComponent 조회
	UFUNCTION(BlueprintPure, Category = "Combat")
	UCombatStatComponent* GetPlayerStat(int32 Index) const;

	// 인덱스로 적 StatComponent 조회
	UFUNCTION(BlueprintPure, Category = "Combat")
	UCombatStatComponent* GetEnemyStat(int32 Index) const;

protected:
	virtual void BeginPlay() override;
};
