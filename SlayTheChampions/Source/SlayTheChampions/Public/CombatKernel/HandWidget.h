#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Card/CardDataTypes.h"
#include "HandWidget.generated.h"

class UScrollBox;
class UButton;

// 취소 버튼 클릭 시 브로드캐스트 (BattleMainWidget이 구독해 패널 숨김 처리)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandWidgetCancel);
// 다음 플레이어 버튼 클릭 시 브로드캐스트 (BattleMainWidget이 구독해 선택 전환)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHandWidgetNextPlayer);
// 카드 클릭 시 브로드캐스트 — CardID를 BattleMainWidget에 전달
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCardSelected, FName, CardID);

/**
 * UHandWidget
 * 플레이어 손패 패널의 C++ 베이스 클래스.
 * WBP_HandPanel이 이 클래스를 부모로 사용한다.
 *
 * 역할:
 *  - CardContainer(ScrollBox)에 카드 위젯 표시
 *  - 취소 / 다음 플레이어 버튼 델리게이트 발행
 *  - ShowHand / ClearHand 공개 API 제공
 *
 * 카드 위젯 생성(WBP_Card 인스턴스화)은 OnShowHand BlueprintImplementableEvent로 BP에 위임.
 */
UCLASS()
class SLAYTHECHAMPIONS_API UHandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ── 델리게이트 ────────────────────────────────────────────────────────
	// 취소 버튼 클릭 시 (BattleMainWidget → 패널 숨기기)
	UPROPERTY(BlueprintAssignable, Category = "Hand")
	FOnHandWidgetCancel OnCancelPressed;

	// 다음 플레이어 버튼 클릭 시 (BattleMainWidget → SelectNextPlayer 호출)
	UPROPERTY(BlueprintAssignable, Category = "Hand")
	FOnHandWidgetNextPlayer OnNextPlayerPressed;

	// 카드 클릭 시 — WBP_HandPanel BP에서 Broadcast, BattleMainWidget이 구독
	// BlueprintCallable 추가: 소유 BP(WBP_HandPanel)에서도 Call(Broadcast) 가능
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Hand")
	FOnCardSelected OnCardSelected;

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
	// ── BindWidget ────────────────────────────────────────────────────────
	// 카드 목록 스크롤박스 — WBP에서 이름 'CardContainer' 일치 필요
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UScrollBox* CardContainer;

	// 취소 버튼 — WBP에서 이름 'BtnCancel' 일치 필요
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnCancel;

	// 다음 플레이어 버튼 — WBP에서 이름 'BtnNextPlayer' 일치 필요
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnNextPlayer;

	virtual void NativeConstruct() override;

private:
	// BtnCancel OnClicked 수신 → OnCancelPressed 브로드캐스트
	UFUNCTION()
	void HandleCancelClicked();

	// BtnNextPlayer OnClicked 수신 → OnNextPlayerPressed 브로드캐스트
	UFUNCTION()
	void HandleNextPlayerClicked();
};
