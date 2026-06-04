#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Card/CardDataTypes.h"
#include "CombatKernel/CombatManager.h"
#include "ActionHistoryWidget.generated.h"

class UVerticalBox;

/**
 * UActionHistoryWidget
 * 카드 사용 히스토리를 최대 10개 표시하는 위젯.
 * WBP_ActionHistory가 이 클래스를 부모로 사용한다.
 *
 * OnActionExecuted 수신 시 OnClearHistory → OnAddEntry*N 순서로 전체 재구성한다.
 * ActionQueue는 턴 초기화 없이 최대 10개를 유지하므로 항상 큐와 동기화된다.
 */
UCLASS()
class SLAYTHECHAMPIONS_API UActionHistoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// CombatManager 수동 설정 — NativeConstruct 자동 탐색 실패 시 BP에서 호출
	UFUNCTION(BlueprintCallable, Category = "History")
	void SetCombatManager(ACombatManager* Manager);

	// 히스토리 전체 초기화 — BP에서 HistoryBox를 ClearChildren 처리
	UFUNCTION(BlueprintImplementableEvent, Category = "History")
	void OnClearHistory();

	// 항목 1개 추가 — BP에서 위젯 생성 후 HistoryBox에 추가
	UFUNCTION(BlueprintImplementableEvent, Category = "History")
	void OnAddEntry(const FCardDataRow& Card, int32 CasterIndex);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UVerticalBox* HistoryBox;

private:
	UPROPERTY()
	ACombatManager* CombatManager = nullptr;

	UFUNCTION()
	void HandleActionExecuted(FCardDataRow Card, int32 CasterIndex);
};
