#include "CombatKernel/EffectManager.h"
#include "CombatKernel/BlockEffect.h"
#include "Unit/Unit.h"
#include "Unit/StatusEffectComponent.h"

void UEffectManager::ApplyEffect(AUnit* Target, EEffectType Type, int32 Value)
{
	if (!Target || Value <= 0) return;

	switch (Type)
	{
		case EEffectType::Block:
		{
			UStatusEffectComponent* SEC = Target->FindComponentByClass<UStatusEffectComponent>();
			if (SEC)
				SEC->ApplyEffect(UBlockEffect::StaticClass(), Value, -1);
			break;
		}
	}
}
