#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatStatWidget.generated.h"

class UTextBlock;
class UCombatStatComponent;
class UStatComponent;

/**
 * 전투원 머리 위에 띄우는 스탯 위젯 (HP / 방어도).
 * CombatantActor의 WidgetComponent 또는 AUnit의 WidgetComponent에 할당.
 * InitWidget() 또는 InitWidgetFromUnit()으로 연결하면 이후 자동 갱신.
 */
UCLASS()
class SLAYTHECHAMPIONS_API UCombatStatWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// CombatantActor용: UCombatStatComponent 연결
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void InitWidget(UCombatStatComponent* InStat);

	// AUnit용: UStatComponent 연결 (Defence 표시 없음 - [임시] StatComponent에 방어도 추가 시 연결)
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void InitWidgetFromUnit(UStatComponent* InStat);

protected:
	// BP에서 이름 맞춰 바인딩 (TextBlock 이름이 정확히 일치해야 함)
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_HP;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Defence;

private:
	// CombatantActor용 스탯 컴포넌트
	UPROPERTY()
	UCombatStatComponent* StatComp;

	// AUnit용 스탯 컴포넌트
	UPROPERTY()
	UStatComponent* UnitStatComp;

	// UCombatStatComponent 델리게이트 핸들러
	UFUNCTION()
	void OnHPChanged(int32 Current, int32 Max);

	UFUNCTION()
	void OnDefenceChanged(int32 Current);

	// UStatComponent 델리게이트 핸들러 (OldValue 무시, NewValue가 현재 HP)
	UFUNCTION()
	void OnUnitHPChanged(int32 OldValue, int32 NewValue);
};
