#include "CombatKernel/HandWidget.h"
#include "Components/HorizontalBox.h"
#include "Card/CardWidget.h"

// PendingCardSource 카드의 스크린 중심 좌표 반환 (화살표 시작점 계산용)
FVector2D UHandWidget::GetPendingCardScreenCenter() const
{
	if (!PendingCardSource) return FVector2D::ZeroVector;
	const FGeometry& Geo = PendingCardSource->GetCachedGeometry();
	// GetAbsolutePosition() = 좌상단, AbsoluteSize * 0.5 = 중심 오프셋
	return Geo.GetAbsolutePosition() + Geo.GetAbsoluteSize() * 0.5f;
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
