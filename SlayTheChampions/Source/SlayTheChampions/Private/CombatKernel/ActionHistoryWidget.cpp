#include "CombatKernel/ActionHistoryWidget.h"
#include "Kismet/GameplayStatics.h"

void UActionHistoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ACombatManager* Found = Cast<ACombatManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ACombatManager::StaticClass()));

	if (Found)
		SetCombatManager(Found);
	else
		UE_LOG(LogTemp, Warning, TEXT("[ActionHistoryWidget] CombatManager not found in level"));
}

void UActionHistoryWidget::NativeDestruct()
{
	if (CombatManager)
	{
		CombatManager->OnActionExecuted.RemoveDynamic(this, &UActionHistoryWidget::HandleActionExecuted);
		CombatManager = nullptr;
	}

	Super::NativeDestruct();
}

void UActionHistoryWidget::SetCombatManager(ACombatManager* Manager)
{
	if (CombatManager)
		CombatManager->OnActionExecuted.RemoveDynamic(this, &UActionHistoryWidget::HandleActionExecuted);

	CombatManager = Manager;

	if (CombatManager)
		CombatManager->OnActionExecuted.AddDynamic(this, &UActionHistoryWidget::HandleActionExecuted);
}

void UActionHistoryWidget::HandleActionExecuted(FCardDataRow Card, int32 CasterIndex)
{
	// 기존 항목 전부 제거 후 현재 큐 전체 재구성
	OnClearHistory();

	if (!CombatManager) return;

	for (const FQueuedAction& Action : CombatManager->GetActionHistory())
		OnAddEntry(Action.Card, Action.CasterIndex);
}
