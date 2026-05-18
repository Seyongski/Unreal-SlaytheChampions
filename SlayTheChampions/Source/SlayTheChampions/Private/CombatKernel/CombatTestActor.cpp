// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatKernel/CombatTestActor.h"
#include "CombatKernel/CombatStatComponent.h"

ACombatTestActor::ACombatTestActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StatComponent = CreateDefaultSubobject<UCombatStatComponent>(TEXT("StatComponent"));
}

void ACombatTestActor::Test_TakeDamage()
{
	StatComponent->TakeDamage(TestDamageAmount);

	UE_LOG(LogTemp, Warning, TEXT("[CombatTest] TakeDamage %d → HP: %d/%d | Defence: %d"),
		TestDamageAmount, StatComponent->CurrentHP, StatComponent->MaxHP, StatComponent->Defence);
}

void ACombatTestActor::Test_AddDefence()
{
	StatComponent->AddDefence(TestDefenceAmount);

	UE_LOG(LogTemp, Warning, TEXT("[CombatTest] AddDefence %d → Defence: %d"),
		TestDefenceAmount, StatComponent->Defence);
}

void ACombatTestActor::Test_Heal()
{
	StatComponent->Heal(TestHealAmount);

	UE_LOG(LogTemp, Warning, TEXT("[CombatTest] Heal %d → HP: %d/%d"),
		TestHealAmount, StatComponent->CurrentHP, StatComponent->MaxHP);
}

void ACombatTestActor::Test_ResetDefence()
{
	StatComponent->ResetDefence();

	UE_LOG(LogTemp, Warning, TEXT("[CombatTest] ResetDefence → Defence: 0"));
}
