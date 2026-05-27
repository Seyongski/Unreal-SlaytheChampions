#include "CombatKernel/HandWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

// 위젯 초기화: 버튼 클릭 이벤트 바인딩
void UHandWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 취소 버튼 바인딩 (BindWidgetOptional이므로 null 체크 필요)
	if (BtnCancel)
		BtnCancel->OnClicked.AddDynamic(this, &UHandWidget::HandleCancelClicked);

	// 다음 플레이어 버튼 바인딩
	if (BtnNextPlayer)
		BtnNextPlayer->OnClicked.AddDynamic(this, &UHandWidget::HandleNextPlayerClicked);
}

// 손패 갱신: 기존 카드 제거 후 BP에 카드 데이터 전달
void UHandWidget::ShowHand(const TArray<FCardDataRow>& Cards)
{
	ClearHand();
	OnShowHand(Cards);
}

// CardContainer의 자식 위젯 전부 제거
void UHandWidget::ClearHand()
{
	if (CardContainer)
		CardContainer->ClearChildren();
}

// 취소 버튼 클릭 → 델리게이트 브로드캐스트
void UHandWidget::HandleCancelClicked()
{
	OnCancelPressed.Broadcast();
}

// 다음 플레이어 버튼 클릭 → 델리게이트 브로드캐스트
void UHandWidget::HandleNextPlayerClicked()
{
	OnNextPlayerPressed.Broadcast();
}
