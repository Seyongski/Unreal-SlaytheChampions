# 레벨 / 전투 셋업 가이드

CombatManager 기반 전투를 **스트리밍 서브레벨** 구조로 굴리기 위한 에디터 설정 정리.
(코드 구조: 퍼시스턴트 MainLevel + 전투 레벨 프리로드 → 하나씩 활성화 → 활성화 시 `BeginCombat`, 전멸 시 `EndCombat`)

---

## 1. 전체 구조

```
MainLevel (퍼시스턴트)
 ├─ NormalMap / EliteMap / BossMap (스트리밍, 프리로드 후 숨김)
 │     └─ 각 레벨에 CombatManager 1개 + PlayerBox/EnemyBox/CameraSlot 배치
 └─ GridMap / ShopLevel / RestLevel ... (CombatManager 없음)

LevelManager(C++)가 레벨을 보이게 함
 → OnLevelShown(BeginPlay 완료) → 그 레벨의 CombatManager.BeginCombat()
 → 전멸 감지 → CombatManager.EndCombat(bWon)
     → BattleMainWidget 제거 + 스폰 유닛 정리 + OnCombatEnded(bWon) 브로드캐스트
 → MainLevel이 OnCombatEnded 구독 → 레벨 숨김 + GridMap 복귀
```

---

## 2. 전투 레벨의 CombatManager 인스턴스 설정 (필수)

각 전투 서브레벨(NormalMap/EliteMap/BossMap 등)에 CombatManager를 1개 배치하고 디테일에서:

| 프로퍼티 | 값 | 비고 |
|---|---|---|
| **bAutoBeginCombat** | **false** | ⚠️ 스트리밍은 반드시 false (LevelManager가 활성화 시 BeginCombat). true면 직접 PIE 테스트용 |
| `EnemyTable` | 적 도감 DataAsset (UEnemyDataTable) | 적 데이터 출처 |
| `Encounter` | 인카운터 DataAsset (UEncounterData) | 이번 전투 등장 EnemyID 목록 |
| `EncounterEnemyIDs` | (비움) | Encounter 없을 때만 쓰는 인라인 목록 |
| `EnemyActorClass` | BP_Enemy | 스폰할 제네릭 적 액터 |
| `PlayerActorClass` | BP_Player | 스폰할 단일 플레이어 액터 |
| `BattleWidgetClass` | WBP_BattleMainWidget | 전투 UI |
| `BattleCameraClass` | BP_BattleCameraActor | 전투 카메라 |
| `PlayerActor_0~2` / `EnemyActor_0~2` | (비움) | 데이터 스폰 시 비움. 채우면 수동 모드 |

**박스/카메라슬롯**: CombatManager의 PlayerBox_0~2 / EnemyBox_0~2 / CameraSlot_* 컴포넌트를 그 레벨 아레나에 맞게 이동·회전.

---

## 3. 적 데이터 (도감 + 인카운터)

- **UEnemyDataTable** (도감): `Entries` 배열에 적별 풀스펙(EnemyID, MaxHP, Pattern, Gimmick, StartingEffects, VisualData) 작성.
- **UEncounterData** (인카운터): `EnemyIDs`에 이번 전투 등장 EnemyID 나열 (최대 3, 박스 순서대로).
- CombatManager가 `Encounter.EnemyIDs` → `EnemyTable.FindByID` → BP_Enemy 스폰 후 `InitializeFromDefinition`으로 데이터 주입.

> BP_Enemy의 `EnemyInitializerComponent.Table`은 비워도 됨 (CombatManager가 직접 주입).

---

## 4. 플레이어 데이터 (PartyInstance)

플레이어는 **단일 BP_Player를 직업별로 스폰**. 인스턴스에 직업 목록을 채워두면 됨.

- **데이터 주입** (메뉴/GameMode BeginPlay 등 전투 진입 전):
  ```
  Get Game Instance Subsystem (PartyInstance)
   → Add Champion (Job: Warrior)   // PawnIndex 0
   → Add Champion (Job: Mage)      // PawnIndex 1
  ```
  배열 순서 = PlayerBox 순서 = PawnIndex.
- CombatManager가 `ChampionJobs`마다 BP_Player 스폰 후:
  - `CardUserComponent.JobClass` = 직업 (덱 로드)
  - `JobComponent.SetJobClass(직업)` (직업 로직 Detail 재생성)

> 타이밍: CombatManager의 InitCombat은 한 틱 미뤄 실행되므로, GameMode BeginPlay에서 `Add Champion` 하면 됨.

---

## 5. BP 컴포넌트 요구사항

**BP_Player** (단일, 직업만 바뀜): `CardUserComponent`, `JobComponent`, `StatComponent`, (필요시 UnitAnimComponent 등). 직업 기본값은 무관 — CombatManager가 덮어씀.

**BP_Enemy** (단일, 데이터 주입): `EnemyInitializerComponent`, `StatComponent`, `NPCBrainComponent`, `IntentComponent`, `UnitAnimComponent`, 머리 위 `WidgetComponent`(MonsterActionWidget). Table/EnemyID는 비워둠.

---

## 6. MainLevel (퍼시스턴트) — 전투 종료 처리

각 전투 레벨 CombatManager의 **`OnCombatEnded(bWon)` 델리게이트를 구독**해서:
- 승리 → RunSystem.AreaCleared → 레벨 숨김 + GridMap 복귀
- 패배 → RunFail 처리

> BattleMainWidget 제거·스폰 유닛 정리는 CombatManager.EndCombat이 이미 처리하므로, MainLevel은 **레벨 숨김/복귀**만 담당.

---

## 7. 흐름 요약

1. MainLevel에서 전투 레벨들 프리로드(숨김) — 이 시점엔 BeginPlay 미실행
2. 방 선택 → 전투 레벨 보이게 → `OnLevelShown` → `BeginCombat()`
3. 스폰(플레이어/적) → 위젯 생성 → 턴 시작
4. 전멸 → `CheckCombatEnd` → `EndCombat(bWon)` → 위젯 삭제 + 유닛 정리 + `OnCombatEnded`
5. MainLevel이 받아서 레벨 숨김 + GridMap 복귀
6. 재방문 시 다시 `OnLevelShown` → `BeginCombat` → 깨끗한 새 전투

---

## 8. 빠른 체크리스트

- [ ] 전투 레벨 CombatManager: `bAutoBeginCombat = false`
- [ ] `EnemyTable` / `Encounter`(또는 `EncounterEnemyIDs`) / `EnemyActorClass` / `PlayerActorClass` 지정
- [ ] PlayerActor/EnemyActor 슬롯 비움
- [ ] PartyInstance에 전투 진입 전 `Add Champion` 호출
- [ ] BP_Player / BP_Enemy 필수 컴포넌트 확인
- [ ] MainLevel이 `OnCombatEnded` 구독 → 레벨 숨김/복귀
- [ ] 직접 PIE 테스트 시에만 `bAutoBeginCombat = true`
