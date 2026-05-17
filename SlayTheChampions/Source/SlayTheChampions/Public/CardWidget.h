#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardDataTypes.h"
#include "CardWidget.generated.h"

class UCardStyleDataAsset;
class UImage;
class UTextBlock;

/**
 * UCardWidget
 *
 * 카드 한 장 위젯 C++ 베이스 클래스.
 * WBP_Card 의 부모 클래스로 사용.
 *
 * 사용법:
 *   1. WBP_Card 를 UCardWidget 을 부모로 생성
 *   2. WBP_Card 에서 아래 BindWidget 변수명과 동일한 위젯 배치
 *   3. SetCardData() 호출하면 자동으로 내용 채워짐
 */
UCLASS()
class SLAYTHECHAMPIONS_API UCardWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    /**
     * 카드 데이터와 직업 스타일을 받아 위젯 전체를 갱신.
     * HandComponent 의 OnHandChanged 등에서 호출.
     *
     * @param InCardData  FCardDataRow 데이터
     * @param InStyle     직업별 UCardStyleDataAsset
     */
    UFUNCTION(BlueprintCallable, Category = "Card|UI")
    void SetCardData(const FCardDataRow& InCardData, UCardStyleDataAsset* InStyle);

    // Blueprint 에서 추가 처리가 필요할 때 오버라이드
    UFUNCTION(BlueprintImplementableEvent, Category = "Card|UI")
    void OnCardDataSet(const FCardDataRow& InCardData);

protected:
    // WBP_Card 에서 반드시 같은 이름으로 위젯 배치
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> BorderImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> GemImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UImage> MainImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> CardNameText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> DescriptionText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> CostText;

private:
    // 현재 세팅된 카드 데이터 (Blueprint 에서 조회 가능)
    UPROPERTY(BlueprintReadOnly, Category = "Card|UI", meta = (AllowPrivateAccess = "true"))
    FCardDataRow CurrentCardData;
};