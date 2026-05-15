// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Unit/CombatTypes.h"
#include "GimmickComponent.generated.h"

class UGimmickData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhaseEntered, const FGimmickPhase&, Phase);

UCLASS( ClassGroup=(Unit), meta=(BlueprintSpawnableComponent) )
class SLAYTHECHAMPIONS_API UGimmickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGimmickComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gimmick")
	UGimmickData* Data = nullptr;

	//각 페이즈가 이미 발동했는지 추적(bOneShot 처리용)
	UPROPERTY(BlueprintReadOnly, Category = "Gimmick")
	TArray<bool> Fired;

	UPROPERTY(BlueprintReadOnly, Category = "Gimmick")
	int32 TurnCounter = 0;

	// CombatManager가 턴 시작 시 호출
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void OnTurnStart();
		
	// UI/연출이 구독. "그가 분노한다!" 같은 연출 트리거용
	UPROPERTY(BlueprintAssignable, Category = "Gimmick")
	FOnPhaseEntered OnPhaseEntered;

protected:
	virtual void BeginPlay() override;

	//StatComponent.OnHPChanged에 자동바인딩 는 아직 구현 x
	/*
	UFUNCTION()
	void HandleHPChanged(float OldHP, float NewHP);
	*/

private:
	void CheckTriggers();
	bool EvaluateTrigger(const FGimmickPhase& Phase) const;
	void EnterPhase(int32 Index);
};
