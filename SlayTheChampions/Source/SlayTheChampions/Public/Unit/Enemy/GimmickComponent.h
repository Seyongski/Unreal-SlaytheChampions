// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Unit/CombatTypes.h"
#include "GimmickComponent.generated.h"


class UStatComponent;
class AUnit;



//기믹이 외부 시스템에 요청하는 Delegate
//CombatManager가 실제처리
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGimmickDamageRequest, ETargetType, TargetType, int32, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGimmickAnnounce, const FText&, Text);

UCLASS( ClassGroup=(Unit), meta=(BlueprintSpawnableComponent) )
class SLAYTHECHAMPIONS_API UGimmickComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGimmickComponent();

	
	// 경과 턴수 OnTurnStart() 마다 1증가 모든 기믹 자식이 공통적으로 사용
	UPROPERTY(BlueprintReadOnly, Category = "Gimmick")
	int32 TurnCounter = 0;

	// CombatManager가 호출
	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void OnTurnStart();

	UFUNCTION(BlueprintCallable, Category = "Gimmick")
	void OnTurnEnd();
		
	/*
	*적 Intent에 표시할 기믹 경고 문구
	* 디자이너가 인스턴스마다 에디터에서 채우기
	* WillTriggerNextTurn() trun일때 AugmentIntent() 가 텍스트를 Intent에 실어 보냄
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Gimmick")
	FText GimmickWarningText;
	
	// ── Intent 보강 API (NPCBrainComponent가 PlanNextAction 마지막에 호출) ──
	/**
	* 다음 턴에 발동할 기믹 정보를 Intent에 미리 추가한다.
	* 패턴이 결정한 1차 Intent는 그대로 두고, GimmickWarning 필드만 채운다.
	* 서브클래스에서 override.
	*/
	void AugmentIntent(FIntent& InOutIntent) const;


	/**
	 * 다가오는 적 턴에 이 기믹이 "경고할 거리"가 있는지.
	 * 턴 기반 기믹(저주/극딜체크/각성 등): GetPredictedTurn()으로 발동 시점을 예측.
	 * 상시형 기믹(폭탄/방패 등): 현재 상태로 판단.
	 * 자식 클래스가 override 하는 유일한 함수.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gimmick")
	virtual bool WillTriggerNextTurn() const { return false; }


	//외부 구독용 Delegate
	UPROPERTY(BlueprintAssignable, Category = "Gimmick")
	FOnGimmickDamageRequest OnGimmickDamageRequest;

	UPROPERTY(BlueprintAssignable, Category = "Gimmick")
	FOnGimmickAnnounce OnGimmickAnnounce;


protected:
	virtual void BeginPlay() override;

	// ── 서브클래스용 virtual hooks ,OnTurnStart/End 내부에서 호출됨
	virtual void OnGimmickTurnStart() {}
	virtual void OnGimmickTurnEnd() {}

	/**
	 * 다음 OnGimmickTurnStart()가 평가하게 될 TurnCounter 예측값.
	 * OnTurnStart()가 ++TurnCounter 후 OnGimmickTurnStart()를 호출하므로 TurnCounter + 1.
	 * 자식의 WillTriggerNextTurn() 구현이 off-by-one 없이 쓰도록 부모가 제공한다.
	 */
	int32 GetPredictedTurn() const { return TurnCounter + 1; }


	//StatComponent.OnHPChanged에 자동바인딩
	UFUNCTION()
	virtual void HandleHPChanged(int32 OldHP, int32 NewHP) {}
	
	//Unit.OnUnitDied에 바인딩
	UFUNCTION()
	virtual void HandleOwnerDied(AUnit* Unit) {}

};
