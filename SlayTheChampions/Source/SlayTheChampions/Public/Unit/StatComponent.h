// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

class AUnit;

// UI와 연결되는 이벤트 선언
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, int32, OldValue, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlockChanged, int32, OldValue, int32, NewValue);

UCLASS( ClassGroup=(Unit), meta=(BlueprintSpawnableComponent) )
class SLAYTHECHAMPIONS_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	float MaxHP = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	float CurrentHP = 100;

	UFUNCTION(BlueprintCallable, Category  = "Stat")
	void TakeDamage(float Damage, AUnit* Instigator);

	UFUNCTION(BlueprintCallable, Category = "Stat")
	void Heal(float Amount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
