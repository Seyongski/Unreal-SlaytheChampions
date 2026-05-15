#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CardDataTypes.h"
#include "PartyMemberPawn.generated.h"

class UHandComponent;

/**
 * APartyMemberPawn
 *
 * Pawn1 / Pawn2 공통 기반 클래스.
 * - HandComponent 를 소유한다.
 * - 직업(JobClass) 을 에디터에서 지정하면
 *   BeginPlay 에서 CardSubsystem 을 통해 해당 직업 카드풀로 덱을 자동 초기화한다.
 * - 전투 규칙(데미지, 코스트 등)은 이 클래스 밖에서 처리한다.
 */
UCLASS()
class SLAYTHECHAMPIONS_API APartyMemberPawn : public APawn
{
    GENERATED_BODY()

public:
    APartyMemberPawn();

protected:
    virtual void BeginPlay() override;

public:
    // ── 컴포넌트 ──────────────────────────────────────────────────────────
    /** 이 Pawn 의 덱·손패·버리기 더미 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Card")
    TObjectPtr<UHandComponent> HandComponent;

    // ── 직업 설정 ─────────────────────────────────────────────────────────
    /**
     * 에디터에서 지정. BeginPlay 시 이 직업에 맞는 카드풀로 덱 초기화.
     * Warrior / Mage / Healer
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    EJobClass JobClass = EJobClass::Warrior;

    /**
     * 덱에 넣을 카드 목록을 직접 지정하고 싶을 때 사용 (선택적).
     * 비어있으면 JobClass 기준으로 CardSubsystem 에서 자동 조회.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    TArray<FName> OverrideDeckNames;

    // ── 덱 조작 ───────────────────────────────────────────────────────────

    /**
     * 덱 초기화 (외부에서 명시적으로 호출 가능).
     * OverrideDeckNames 가 있으면 그걸 사용, 없으면 JobClass 기반 자동 조회.
     */
    UFUNCTION(BlueprintCallable, Category = "Card")
    void InitializeDeck();

    /** 편의 함수 - HandComponent 로 포워딩 */
    UFUNCTION(BlueprintCallable, Category = "Card")
    void DrawStartOfTurn();

    UFUNCTION(BlueprintCallable, Category = "Card")
    bool PlayCard(FName CardName);

    UFUNCTION(BlueprintCallable, Category = "Card")
    void DiscardHand();

    // ── 조회 (Blueprint 노출용) ───────────────────────────────────────────
    UFUNCTION(BlueprintPure, Category = "Card")
    TArray<FName> GetHand() const;

    UFUNCTION(BlueprintPure, Category = "Card")
    int32 GetDrawPileCount() const;

    UFUNCTION(BlueprintPure, Category = "Card")
    int32 GetDiscardPileCount() const;
};