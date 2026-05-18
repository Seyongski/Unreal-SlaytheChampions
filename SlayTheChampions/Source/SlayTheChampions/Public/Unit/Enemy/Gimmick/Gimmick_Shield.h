// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unit/Enemy/GimmickComponent.h"
#include "Gimmick_Shield.generated.h"

/**
 * 일정 스택만큼 피해를 흡수하는 방패기믹
 * 스택이 있는 동안 모든 피해를 1로 제한, 히트당 1스택 소모
 * 연타공격 을 요구하는 기믹
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldStackChanged, int32, OldStacks, int32, NewStacks);


UCLASS(ClassGroup = (Unit), meta = (BlueprintSpawnableComponent))
class SLAYTHECHAMPIONS_API UGimmick_Shield : public UGimmickComponent
{
	GENERATED_BODY()
	
public:
	//초기 스택 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick|Shield")
	int32 MaxStacks = 10;

	//현재 스택 수
	UPROPERTY(BlueprintReadOnly, Category = "Gimmick|Shield")
	int32 CurrentStacks = 0;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gimmick|Shield")
	bool HasShield() const { return CurrentStacks > 0; }

	// UI가 스택 변화를 구독
	UPROPERTY(BlueprintAssignable, Category = "Gimmick|Shield")
	FOnShieldStackChanged OnShieldStackChanged;

protected:

};
