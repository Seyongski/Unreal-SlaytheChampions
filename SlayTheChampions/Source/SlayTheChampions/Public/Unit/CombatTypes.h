// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CombatTypes.generated.h"

class AUnit;

//진영 구분
UENUM(BlueprintType)
enum class ETeam : uint8
{
    Ally    UMETA(DisplayName = "Ally"),
    Enemy   UMETA(DisplayName = "Enemy"),
    Neutral UMETA(DisplayName = "Neutral")
};

//  적 행동 카테고리
//  EEffectType이 None일 때만 디자이너가 직접 선택 (Attack/Defend).
//  EEffectType이 Buff_/Debuff_이면 FEnemyAction::ResolveIntentKind()가 자동 도출.

UENUM(BlueprintType)
enum class EIntentKind : uint8
{
    Attack UMETA(DisplayName = "Attack"),
    Defend UMETA(DisplayName = "Defend"),
    Buff    UMETA(DisplayName = "Buff"),
    Debuff  UMETA(DisplayName = "Debuff"),
    Unknown UMETA(DisplayName = "Unknown")
};

//상태효과 종류 (단일 Enum)

// ──────────────────────────────────────────────
//  상태 효과 종류 (단일 enum)
//  - 0          : None (Attack/Defend 액션 또는 미사용)
//  - 100~199    : Buff
//  - 200~299    : Debuff
//  값 범위로 IsBuff/IsDebuff를 분기한다.
//  Buff_Block은 의도적으로 제외 — 방어도는 CombatStatComponent 담당.
// ──────────────────────────────────────────────
UENUM(BlueprintType)
enum class EEffectType : uint8
{
    None = 0   UMETA(DisplayName = "None"),

    // ── Buff (100~199) ─────────────────────
    Buff_AttackUp = 100 UMETA(DisplayName = "공격력 증가"),
    Buff_DefenseUp = 101 UMETA(DisplayName = "방어력 증가"),
    Buff_Regen = 102 UMETA(DisplayName = "재생"),

    // ── Debuff (200~299) ───────────────────
    Debuff_Weak = 200 UMETA(DisplayName = "약화"),
    Debuff_Vulnerable = 201 UMETA(DisplayName = "취약"),
    Debuff_Burn = 202 UMETA(DisplayName = "작열"),
    Debuff_Frail = 203 UMETA(DisplayName = "허약")
};
// ──────────────────────────────────────────────
//  EEffectType 헬퍼
//  범위 기반: enum 항목 추가 시 IsBuff/IsDebuff 수정 불필요
// ──────────────────────────────────────────────
namespace EffectTypeHelpers
{
    FORCEINLINE bool IsBuff(EEffectType Type)
    {
        const uint8 V = static_cast<uint8>(Type);
        return V >= 100 && V < 200;
    }

    FORCEINLINE bool IsDebuff(EEffectType Type)
    {
        const uint8 V = static_cast<uint8>(Type);
        return V >= 200 && V < 300;
    }

    FORCEINLINE EIntentKind ToIntentKind(EEffectType Type)
    {
        if (IsBuff(Type))   return EIntentKind::Buff;
        if (IsDebuff(Type)) return EIntentKind::Debuff;
        return EIntentKind::Unknown;
    }
}

//// ──────────────────────────────────────────────
////  런타임 상태 효과 인스턴스 (USTRUCT)
////  UStatusEffectComponent.Active 배열에 보관.
////  UObject가 아닌 POD라서 메모리/GC 부담 없음.
//// ──────────────────────────────────────────────
USTRUCT(BlueprintType)
struct SLAYTHECHAMPIONS_API FStatusEffects
{
    GENERATED_BODY()

    /** 효과 자신의 정체성 (UI 아이콘/dispatch 키) */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    EEffectType EffectType = EEffectType::None;

    /** 현재 값 (스택/강도). 의미는 EffectType마다 다름. */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    int32 Value = 0;

    /** 턴마다 Value에 더해지는 값. 디버프는 보통 -1. */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    int32 DeltaPerTurn = 0;

    /** 남은 적용 횟수. -1 = 무한, 0 = 즉시 제거. 제거 판정은 Count만 사용. */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    int32 Count = -1;

    /** 자기 턴 시작 시 무조건 제거. (Block 같은 자기 강화 패턴용) */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    bool bResetOnTurnStart = false;

    /** Value 클램프 최솟값. 제거 판정에는 사용하지 않음. */
    UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Effect")
    int32 FloorValue = -2147483648; // INT32_MIN
};

// UI가 이 구조체만 읽어서 머리위에 아이콘을 표시하게 설계
USTRUCT(BlueprintType)
struct SLAYTHECHAMPIONS_API FIntent
{
    GENERATED_BODY()

    // ── 패턴이 결정하는 1차 의도 (모든 적 공통) ─────────────
    UPROPERTY(BlueprintReadOnly) EIntentKind Kind = EIntentKind::Unknown;
    UPROPERTY(BlueprintReadOnly) int32       Value = 0;  // 예상 데미지 or 블록량
    UPROPERTY(BlueprintReadOnly) int32       Hits = 1;
    UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AUnit> Target;
    UPROPERTY(BlueprintReadOnly) FText       DisplayText;

    // ── 보스 기믹이 보강하는 추가 정보 (보스 전용, 선택) ───
    /** 다음 턴에 기믹이 발동할 예정인지 여부. UI가 경고 아이콘 표시용으로 사용. */
    UPROPERTY(BlueprintReadOnly) bool        bHasGimmickWarning = false;
    /** 기믹 경고 텍스트 (예: "다음 턴 분노 발동", "방패 활성화 중") */
    UPROPERTY(BlueprintReadOnly) FText       GimmickWarningText;
};



UENUM(BlueprintType)
enum class ETargetType : uint8
{
    SingleEnemy UMETA(DisplayName = "Single Enemy"),
    ALlEnemies UMETA(DisplayName = "All Enemies"),
    Self UMETA(Displayname = "Self"),
    RamdomAlly UMETA(DisplayName = "Random Ally"),
    AllAlies UMETA(DisplayName = "All Alies")
};

UENUM(BlueprintType)
enum class EPatternMode : uint8
{
    Sequential UMETA(DisplayName = "Sequential"), //순서대로 반복
    Weighted UMETA(DisplayName = "Weighted")//가중치랜덤
};

// 적 행동 한 개의 정의. EnemyPatternData 배열에 저장됨.
USTRUCT(BlueprintType)
struct SLAYTHECHAMPIONS_API FEnemyAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) EIntentKind  IntentKind = EIntentKind::Attack;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32        Value = 0;   // 데미지 or 블록량
    UPROPERTY(EditAnywhere, BlueprintReadOnly) int32        Hits = 1;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) ETargetType  TargetType = ETargetType::SingleEnemy;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float        Weight = 1.f;   // Weighted 모드에서만 사용
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText        DisplayName;
};

UENUM(BlueprintType)
enum class EGimmickTrigger : uint8
{
    HPThresholdBelow UMETA(DisplayName = "HP Below Threshold"), // HP가 X% 이하
    TurnCountReached UMETA(DisplayName = "Turn Count Reached"), // N턴 경과
    OnDamaged        UMETA(DisplayName = "On Damaged")          // 피격 시
};

USTRUCT(BlueprintType)
struct SLAYTHECHAMPIONS_API FGimmickPhase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName           PhaseName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EGimmickTrigger Trigger = EGimmickTrigger::HPThresholdBelow;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) float           TriggerValue = 0.5f; // HP면 0~1 비율, Turn이면 턴 수
    //UPROPERTY(EditAnywhere, BlueprintReadOnly) UEnemyPatternData* SwapToPattern = nullptr;
    //UPROPERTY(EditAnywhere, BlueprintReadOnly) TArray<TSubclassOf<UStatusEffect>> ApplyOnEnter;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText           AnnounceText;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) bool            bOneShot = true; // true면 한 번만 발동
};

