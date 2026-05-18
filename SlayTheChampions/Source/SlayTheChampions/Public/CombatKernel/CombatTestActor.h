// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatTestActor.generated.h"

class UCombatStatComponent;

UCLASS()
class SLAYTHECHAMPIONS_API ACombatTestActor : public AActor
{
	GENERATED_BODY()

public:
	ACombatTestActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UCombatStatComponent* StatComponent;

	// 테스트용 조작값 (에디터 Details에서 설정)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestDamageAmount = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestDefenceAmount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 TestHealAmount = 10;

	// BP에서 직접 호출 가능한 테스트 함수
	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test_TakeDamage();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test_AddDefence();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test_Heal();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void Test_ResetDefence();
};
