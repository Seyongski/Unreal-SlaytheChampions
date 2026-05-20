#include "CombatKernel/BlockEffect.h"

void UBlockEffect::OnTurnEnd()
{
	Stacks = 0;
	// Duration은 건드리지 않음 (-1 유지 = 효과 자체는 제거 안 됨)
}
