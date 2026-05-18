// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatKernel/CombatManager.h"
#include "CombatKernel/CombatStatComponent.h"

ACombatManager::ACombatManager()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ACombatManager::BeginPlay()
{
	Super::BeginPlay();
	InitCombat();
}

void ACombatManager::InitCombat()
{
	// 플레이어 Role 세팅
	for (int32 i = 0; i < PlayerActors.Num(); i++)
	{
		AActor* Actor = PlayerActors[i];
		if (!Actor) continue;

		if (UCombatStatComponent* Stat = Actor->FindComponentByClass<UCombatStatComponent>())
		{
			Stat->Role = (i == 0) ? ECombatRole::Player0 : ECombatRole::Player1;
		}

		UE_LOG(LogTemp, Warning, TEXT("[CombatManager] Player[%d] = %s"), i, *Actor->GetName());
	}

	// 적 Role 세팅
	for (int32 i = 0; i < EnemyActors.Num(); i++)
	{
		AActor* Actor = EnemyActors[i];
		if (!Actor) continue;

		if (UCombatStatComponent* Stat = Actor->FindComponentByClass<UCombatStatComponent>())
		{
			Stat->Role = ECombatRole::Enemy;
		}

		UE_LOG(LogTemp, Warning, TEXT("[CombatManager] Enemy[%d] = %s"), i, *Actor->GetName());
	}
}

UCombatStatComponent* ACombatManager::GetPlayerStat(int32 Index) const
{
	if (!PlayerActors.IsValidIndex(Index) || !PlayerActors[Index]) return nullptr;
	return PlayerActors[Index]->FindComponentByClass<UCombatStatComponent>();
}

UCombatStatComponent* ACombatManager::GetEnemyStat(int32 Index) const
{
	if (!EnemyActors.IsValidIndex(Index) || !EnemyActors[Index]) return nullptr;
	return EnemyActors[Index]->FindComponentByClass<UCombatStatComponent>();
}
