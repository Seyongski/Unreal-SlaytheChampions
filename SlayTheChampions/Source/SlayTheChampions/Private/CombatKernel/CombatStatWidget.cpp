#include "CombatKernel/CombatStatWidget.h"
#include "CombatKernel/CombatStatComponent.h"
#include "Unit/StatComponent.h"
#include "Components/TextBlock.h"

void UCombatStatWidget::InitWidget(UCombatStatComponent* InStat)
{
	if (!InStat) return;
	StatComp = InStat;

	StatComp->OnHPChanged.AddDynamic(this, &UCombatStatWidget::OnHPChanged);
	StatComp->OnDefenceChanged.AddDynamic(this, &UCombatStatWidget::OnDefenceChanged);

	// 초기값 즉시 표시
	OnHPChanged(StatComp->CurrentHP, StatComp->MaxHP);
	OnDefenceChanged(StatComp->Defence);
}

void UCombatStatWidget::OnHPChanged(int32 Current, int32 Max)
{
	if (Text_HP)
		Text_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Current, Max)));
}

void UCombatStatWidget::OnDefenceChanged(int32 Current)
{
	if (Text_Defence)
		Text_Defence->SetText(FText::FromString(FString::Printf(TEXT("DEF %d"), Current)));
}

void UCombatStatWidget::InitWidgetFromUnit(UStatComponent* InStat)
{
	if (!InStat) return;
	UnitStatComp = InStat;

	UnitStatComp->OnHPChanged.AddDynamic(this, &UCombatStatWidget::OnUnitHPChanged);

	// 초기값 즉시 표시
	OnUnitHPChanged(0, UnitStatComp->CurrentHP);
}

void UCombatStatWidget::OnUnitHPChanged(int32 OldValue, int32 NewValue)
{
	// [임시] Defence는 UStatComponent에 없으므로 숨김 처리
	if (Text_Defence)
		Text_Defence->SetVisibility(ESlateVisibility::Collapsed);

	if (Text_HP && UnitStatComp)
		Text_HP->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), NewValue, UnitStatComp->MaxHP)));
}
