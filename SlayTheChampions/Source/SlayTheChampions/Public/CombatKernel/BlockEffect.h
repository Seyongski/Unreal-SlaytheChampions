#pragma once

#include "CoreMinimal.h"
#include "Unit/StatusEffect.h"
#include "BlockEffect.generated.h"

UCLASS()
class SLAYTHECHAMPIONS_API UBlockEffect : public UStatusEffect
{
	GENERATED_BODY()

public:
	// 턴 시작 시 방어도 전부 리셋
	virtual void OnTurnEnd() override;
};
