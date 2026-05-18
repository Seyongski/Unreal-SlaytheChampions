// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatStatComponent.generated.h"

UENUM(BlueprintType)
enum class ECombatRole : uint8
{
	Player0 UMETA(DisplayName = "Player 0"),
	Player1 UMETA(DisplayName = "Player 1"),
	Enemy   UMETA(DisplayName = "Enemy"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged,    int32, Current, int32, Max);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FOnDefenceChanged, int32, Current);

UCLASS(ClassGroup = "Combat", meta = (BlueprintSpawnableComponent))
class SLAYTHECHAMPIONS_API UCombatStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatStatComponent();

	// ── 역할 ────────────────────────────────────────────────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	ECombatRole Role = ECombatRole::Enemy;

	// ── 스탯 ────────────────────────────────────────────────

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 MaxHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 CurrentHP = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Defence = 0;

	// ── 이벤트 (BP에서 UI 갱신용) ───────────────────────────

	UPROPERTY(BlueprintAssignable, Category = "Stats|Event")
	FOnHPChanged OnHPChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats|Event")
	FOnDefenceChanged OnDefenceChanged;

	// ── 조작 함수 ───────────────────────────────────────────

	// 피해 적용 (방어도 먼저 소모, 나머지 HP 차감)
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void TakeDamage(int32 Amount);

	// 방어도 추가
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddDefence(int32 Amount);

	// HP 회복 (MaxHP 초과 불가)
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void Heal(int32 Amount);

	// 턴 시작 시 방어도 초기화
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void ResetDefence();

	// ── 조회 함수 ───────────────────────────────────────────

	UFUNCTION(BlueprintPure, Category = "Stats")
	bool IsDead() const { return CurrentHP <= 0; }

protected:
	virtual void BeginPlay() override;
};
