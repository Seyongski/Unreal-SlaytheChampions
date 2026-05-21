//// Fill out your copyright notice in the Description page of Project Settings.
//
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "Unit/CombatTypes.h"
#include "StatusEffectComponent.generated.h"

class AUnit;
class UStatusEffectmap;

//UI용
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectApplied, UStatusEffect*, Effect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, TSubclassOf<UStatusEffect>, EffectClass);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectValueChanged, const FStatusEffects&, Effect);

UCLASS( ClassGroup=(Unit), meta=(BlueprintSpawnableComponent) )
class SLAYTHECHAMPIONS_API UStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusEffectComponent();


	// 현재 활성된 효과들 EffectType 당 0개 또는 1개(중복시 스택 누적)
	UPROPERTY(BlueprintReadOnly, Category = "StatusEffect")
	TArray<FStatusEffects> AActive;

	/**
	 * 효과 매핑 DataAsset.
	 * 에디터에서 직접 지정하거나, GameInstance/Subsystem에서 주입.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StatusEffect")
	UStatusEffectmap* EffectMap = nullptr;

	//효과 조작
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
	void AApplyEffect(EEffectType Type, int32 Value, int32 Duration);

	/** 특정 종류의 효과를 즉시 제거. */
	UFUNCTION(BlueprintCallable, Category = "StatusEffect")
	void RRemoveEffect(EEffectType Type);

	/** 효과 보유 여부. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatusEffect")
	bool HHasEffect(EEffectType Type) const;

	/** 효과의 현재 Value. 없으면 0. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "StatusEffect")
	int32 GGetEffectValue(EEffectType Type) const;

	/** C++ 전용 포인터 조회. 효과를 직접 수정하고 싶을 때. nullptr 가능. */
	FStatusEffects* FFindEffect(EEffectType Type);
	const FStatusEffects* FFindEffect(EEffectType Type) const;


private:
	/** Value를 FloorValue로 클램프하는 헬퍼. */
	void ClampValue(FStatusEffects& Effect) const;
protected:





	

	


	UPROPERTY(BlueprintAssignable) FOnEffectApplied OnEffectApplied;
	UPROPERTY(BlueprintAssignable) FOnEffectRemoved OnEffectRemoved;

	//UPROPERTY(BlueprintAssignable) FOnEffectValueChanged OnEffectValueChanged;
	
};


