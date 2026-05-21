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


//적 행동 열겨형
UENUM(BlueprintType)
enum class EIntentKind : uint8
{
    Attack UMETA(DisplayName = "Attack"),
    Defend UMETA(DisplayName = "Defend"),
    Buff    UMETA(DisplayName = "Buff"),
    Debuff  UMETA(DisplayName = "Debuff"),
    Unknown UMETA(DisplayName = "Unknown")
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

