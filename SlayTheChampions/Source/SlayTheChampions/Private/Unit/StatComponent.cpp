// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UStatComponent::TakeDamage(float Damage, AUnit* Instigator)
{
	//데미지가 들어와 HP가 다는 함수 구현부
}

void UStatComponent::Heal(float Amount)
{
	//힐이 들어와 체력이 차는 함수 구현부
}

// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


