# BP_BattleCameraActor — 카메라 이동 로직

전투 화면에서 카메라를 슬롯(Default / 각 플레이어) 사이로 부드럽게 이동시키는 액터의 동작 명세.
실제 로직은 BP_BattleCameraActor 블루프린트 안에 있으며, C++(CombatKernel)는 델리게이트 브로드캐스트로 이동을 "요청"만 한다.

---

## 1. 구성 요소

- **카메라 슬롯(Scene Component / Arrow)**: 카메라가 멈출 목표 위치·회전을 나타내는 컴포넌트.
  - `CameraSlot_Default` — 메인 전투 화면(아무도 선택 안 한 상태)의 기본 시점.
  - `CameraSlot_Player0/1/2` — 각 플레이어 선택 시 시점.
  - 슬롯은 `CombatManager`(또는 카메라 액터)가 보유하며, 레벨 인스턴스에서 반드시 할당되어 있어야 한다. (None이면 IsValid에서 걸러짐)
- **카메라 액터 본체(self)**: `Set Actor Location/Rotation`으로 실제로 움직이는 대상.

---

## 2. 상태 변수

| 변수 | 타입 | 용도 |
|------|------|------|
| `Start Location` / `Start Rotation` | Vector / Rotator | 이동 시작 시점의 액터 위치·회전 (Lerp의 A) |
| `Target Location` / `Target Rotation` | Vector / Rotator | 목표 슬롯의 위치·회전 (Lerp의 B) |
| `Camera Moving` | bool | 이동 중 여부. UI 입력 차단 등에 사용 (`HandleBackClicked`가 참조) |
| `LastTargetSlot` | Scene Component 참조 | **멱등 게이트용** — 마지막으로 명령받은 목표 슬롯 |

---

## 3. CameraSet 함수 (멱등 게이트 포함)

"이 슬롯으로 카메라를 이동하라"는 단일 진입점. **같은 슬롯으로의 중복 명령은 무시**한다.

```
CameraSet(Camera : Scene Component)
  │
  ├─ Branch: IsValid(Camera)?
  │     False → Return                      ; 슬롯 미할당 방어
  │     True  ↓
  │
  ├─ Branch: (Camera == LastTargetSlot)?     ; ★ 멱등 게이트
  │     True  → Return                       ; 이미 같은 목표 → 아무것도 안 함 (타임라인 리셋 X)
  │     False ↓
  │
  ├─ SET LastTargetSlot   = Camera
  ├─ SET Start Location   = Get Actor Location(self)     ; 현재 위치에서 출발
  ├─ SET Start Rotation   = Get Actor Rotation(self)
  ├─ SET Target Location  = Get World Location(Camera)   ; 슬롯 위치로
  ├─ SET Target Rotation  = Get World Rotation(Camera)
  ├─ SET Camera Moving    = true
  └─ Timeline → Play from Start
```

### 멱등 게이트가 핵심인 이유
`Play from Start`는 호출될 때마다 타임라인을 **시간 0(알파 0)으로 강제 리셋**한다.
게이트가 없으면, 같은 목표(Default)로의 명령이 한 프레임에 두 번 들어올 때:
1. 첫 명령: `Start = Player0`, `Target = Default` → 이동 시작
2. 둘째 명령: `Start = (이동 중인 어긋난 위치)`, `Target = Default` → 타임라인 0으로 재리셋
   → 어긋난 위치에서 다시 Lerp → **dip(아래로 갔다 올라옴) 발생**

게이트가 있으면 둘째 명령은 `Camera == LastTargetSlot`이라 즉시 Return → 리셋 없음 → dip 없음.
**트리거가 몇 개든, OnPhaseChanged 설정을 어떻게 바꾸든 안전**해진다.

---

## 4. Timeline 이동

선형 커브 (0,0) → (0.30s, 1.0). 알파를 두 Lerp에 공급.

```
Timeline.Update (Alpha)
  ├─ Set Actor Location = Lerp(Vector)(Start Location, Target Location, Alpha)
  ├─ Set Actor Rotation = Lerp(Rotator)(Start Rotation, Target Rotation, Alpha, ShortestPath=true)
  └─ Branch: (Alpha >= 0.9)?
        True → SET Camera Moving = false        ; 플래그만 끔 (타임라인은 계속 1.0까지 진행)
        False → (없음)
```

- 알파가 1.0에 도달하면 `Lerp(Start, Target, 1.0) = Target` → 카메라가 슬롯에 **정확히 안착**.
- 0.9는 타임라인을 멈추지 않는다. 단지 입력 차단 플래그를 조금 일찍 풀어줄 뿐.

---

## 5. 이동을 트리거하는 이벤트 (CombatManager 델리게이트)

| 델리게이트 | 발생 시점 | 카메라 동작 |
|-----------|----------|------------|
| `OnBattlePlayerSelected(Unit)` | 플레이어 슬롯 클릭 | `CameraSet(해당 플레이어 슬롯)` |
| `OnPhaseChanged(NewPhase)` | 전투 페이즈 변경 | `CameraSet(CameraSlot_Default)` (현재 무조건) |
| `OnCameraReturnToDefault` | Back 버튼(플레이어 선택 해제) | `CameraSet(CameraSlot_Default)` |
| `OnTargetingStateChanged(bTargeting, ...)` | 카드 타겟 지정 시작/취소 | 타겟 중: 확대 / 취소: 플레이어 슬롯 복귀 |

> **C++ 측 주의**: `HandleEndTurnClicked`는 `EndPlayerActionPhase()`로 페이즈를 바꾸므로
> `OnPhaseChanged`가 카메라 복귀를 담당한다. 과거에 여기서 `OnCameraReturnToDefault`도 함께
> 브로드캐스트했으나, 멱등 게이트가 없을 땐 더블 트리거로 dip을 유발했다. (현재 제거됨)
> **멱등 게이트 적용 후에는** 두 브로드캐스트가 겹쳐도 안전하므로, 필요하면 belt-and-suspenders로
> end-turn에서 `OnCameraReturnToDefault`를 다시 브로드캐스트해 복귀를 보장해도 된다.

---

## 6. 흐름 예시

### 플레이어 선택 → 턴 종료
```
플레이어0 클릭
  → OnBattlePlayerSelected(P0)
  → CameraSet(Player0): LastTargetSlot(Default→Player0) 변경 → 통과 → Default→Player0 이동

턴 종료
  → EndPlayerActionPhase() → OnPhaseChanged(EnemyPhase)
  → CameraSet(Default): LastTargetSlot(Player0→Default) 변경 → 통과 → Player0→Default 이동
  (※ 만약 OnCameraReturnToDefault도 같이 발생하면:
     CameraSet(Default) 재호출 → LastTargetSlot == Default → Return → 무시, dip 없음)
```

### 이미 Default인데 페이즈가 또 바뀜
```
OnPhaseChanged(...) → CameraSet(Default)
  → LastTargetSlot == Default → Return → 아무 움직임 없음 (정지 유지)
```

---

## 7. 향후 OnPhaseChanged를 조건부로 바꿀 때

`OnPhaseChanged`에서 특정 페이즈만 `CameraSet`을 호출하도록 바꾸더라도:
- 멱등 게이트가 있으므로 중복 호출 걱정 없음.
- 단, **턴 종료로 진입하는 페이즈가 제외되면** 그 경로의 카메라 복귀가 사라지므로,
  end-turn에서 `OnCameraReturnToDefault` 브로드캐스트를 복원해 복귀를 보장할 것.
