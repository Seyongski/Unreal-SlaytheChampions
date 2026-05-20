// Fill out your copyright notice in the Description page of Project Settings.


#include "Unit/StatComponent.h"
#include "Unit/Unit.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UStatComponent::TakeDamage(int32 Damage, AUnit* Instigator)
{
	//�������� ���� HP�� �ٴ� �Լ� ������
	//������ 0�̰ų� ���� HP�� 0�����̸� ����
	if (Damage <= 0 || CurrentHP <= 0) return;

	//HP ����
	const int32 OldHP = CurrentHP;
	CurrentHP = FMath::Max(0, CurrentHP - Damage);

	UE_LOG(LogTemp, Warning, TEXT("[StatComponent] TakeDamage=%d HP: %d -> %d"), Damage, OldHP, CurrentHP);

	//��ե닔ĳ��Ʈ
	OnHPChanged.Broadcast(OldHP, CurrentHP);

	//HP�� 0�̶��
	if (CurrentHP <= 0)
	{
		if (AUnit* Unit = Cast<AUnit>(GetOwner()))
		{
			Unit->HandleDeath();
		}
	}
}

void UStatComponent::Heal(int32 Amount)
{
	//���� ���� ü���� ���� �Լ� ������
	//������ 0�̰ų�, ����ü���� 0�̰ų�, �ִ�ü�� �̻��϶�
	if (Amount <= 0 || CurrentHP <= 0 || CurrentHP >= MaxHP) return;

	const int32 OldHP = CurrentHP;
	CurrentHP = FMath::Min(MaxHP, CurrentHP + Amount);
	if (OldHP != CurrentHP)
	{
		OnHPChanged.Broadcast(OldHP, CurrentHP);
	}
}

// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
	// ...
	
}


