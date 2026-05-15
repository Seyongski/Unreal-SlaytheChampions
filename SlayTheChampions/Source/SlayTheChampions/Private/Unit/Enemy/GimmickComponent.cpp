// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/Enemy/GimmickComponent.h"

// Sets default values for this component's properties
UGimmickComponent::UGimmickComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UGimmickComponent::OnTurnStart()
{
}

void UGimmickComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGimmickComponent::CheckTriggers()
{
}

bool UGimmickComponent::EvaluateTrigger(const FGimmickPhase& Phase) const
{
	return false;
}

void UGimmickComponent::EnterPhase(int32 Index)
{
}


