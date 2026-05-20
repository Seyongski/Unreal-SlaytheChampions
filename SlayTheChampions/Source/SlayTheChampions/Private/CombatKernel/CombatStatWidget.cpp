#include "CombatKernel/CombatStatWidget.h"
#include "CombatKernel/BlockEffect.h"
#include "Unit/StatComponent.h"
#include "Unit/StatusEffectComponent.h"
#include "Components/TextBlock.h"

void UCombatStatWidget::InitWidgetFromUnit(UStatComponent* InStat)
{
	if (!InStat) return;
	UnitStatComp = InStat;

	UnitStatComp->OnHPChanged.AddDynamic(this, &UCombatStatWidget::OnUnitHPChanged);

	// 초기값 즉시 표시
	OnUnitHPChanged(0, UnitStatComp->CurrentHP);
}

void UCombatStatWidget::InitBlockDisplay(UStatusEffectComponent* InSEC)
{
	if (!InSEC) return;
	InSEC->OnEffectApplied.AddDynamic(this, &UCombatStatWidget::OnBlockEffectApplied);

	// 초기값 표시
	if (Text_Defence)
		Text_Defence->SetText(FText::FromString(TEXT("0")));
}

void UCombatStatWidget::OnUnitHPChanged(int32 OldValue, int32 NewValue)
{
	if (Text_HP && UnitStatComp)
		Text_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), NewValue, UnitStatComp->MaxHP)));
}

void UCombatStatWidget::OnBlockEffectApplied(UStatusEffect* Effect)
{
	if (!Effect || !Effect->IsA(UBlockEffect::StaticClass())) return;

	if (Text_Defence)
		Text_Defence->SetText(FText::AsNumber(Effect->Stacks));
}
