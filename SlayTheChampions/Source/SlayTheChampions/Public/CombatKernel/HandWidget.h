#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Card/CardDataTypes.h"
#include "HandWidget.generated.h"

class UHorizontalBox;
class UCardWidget;

// 카드 클릭 시 브로드캐스트 — CardID를 BattleMainWidget에 전달
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardSelected, FName, CardID);

/**
 * UHandWidget
 * 플레이어 손패 패널의 C++ 베이스 클래스.
 * WBP_HandPanel이 이 클래스를 부모로 사용한다.
 *
 * 역할:
 *  - CardContainer(ScrollBox)에 카드 위젯 표시
 *  - ShowHand / ClearHand 공개 API 제공
 *
 * 버튼(EndTurn, NextPlayer)은 WBP_BattleMainWidget에서 직접 관리한다.
 */
UCLASS()
class SLAYTHECHAMPIONS_API UHandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 카드 클릭 시 — WBP_HandPanel BP에서 Broadcast, BattleMainWidget이 구독
	// BlueprintCallable 추가: 소유 BP(WBP_HandPanel)에서도 Call(Broadcast) 가능
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Hand")
	FOnCardSelected OnCardSelected;

	// ── 화살표 앵커 ──────────────────────────────────────────────────────
	// 카드 클릭 직전 WBP_HandPanel BP에서 이 값을 설정 → 화살표 시작점 계산에 사용
	UPROPERTY(BlueprintReadWrite, Category = "Hand")
	UCardWidget* PendingCardSource = nullptr;

	// PendingCardSource 카드의 스크린 중심 좌표 반환 (화살표 시작점)
	// PendingCardSource가 nullptr이면 FVector2D::ZeroVector 반환
	UFUNCTION(BlueprintCallable, Category = "Hand")
	FVector2D GetPendingCardScreenCenter() const;

	// ── 공개 API ─────────────────────────────────────────────────────────
	// 손패 갱신: 기존 카드 제거 후 OnShowHand 브로드캐스트
	UFUNCTION(BlueprintCallable, Category = "Hand")
	void ShowHand(const TArray<FCardDataRow>& Cards);

	// 카드 컨테이너 비우기
	UFUNCTION(BlueprintCallable, Category = "Hand")
	void ClearHand();

	// ── BP 구현부 ─────────────────────────────────────────────────────────
	// WBP_HandPanel에서 카드 위젯(WBP_Card)을 생성하고 CardContainer에 추가
	UFUNCTION(BlueprintImplementableEvent, Category = "Hand")
	void OnShowHand(const TArray<FCardDataRow>& Cards);

protected:
	// 카드 목록 가로 박스 — WBP에서 이름 'CardContainer' 일치 필요
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* CardContainer;
};
