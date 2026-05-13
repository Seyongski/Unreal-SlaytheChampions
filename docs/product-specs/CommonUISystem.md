# CommonUISystem.md

## Common UI System

## 목적

Common UI System은 여러 화면에서 공통으로 쓰이는 UI와 입력 모드 전환을 담당한다.

전투 HUD처럼 특정 기능에 종속된 UI는 해당 시스템 문서에서 관리하고, Pause Menu, Option Menu, Confirm Popup, Loading UI처럼 전역적으로 필요한 UI는 이 문서에서 관리한다.

## 핵심 책임

- Pause Menu 열기/닫기
- Option Menu 열기/닫기
- Confirm Popup 표시
- Loading UI 표시
- 공통 알림/토스트 표시
- Widget Stack 관리
- 입력 모드 전환 요청
- UI Only, Game Only, Game + UI 상태 관리

## 직접 처리하지 않는 것

- Battle HUD 내부 카드 표시
- 카드 타겟팅 UI
- 스테이지 그리드 노드 표시
- 보상 카드 선택 로직
- 씬 로드/언로드
- 게임 상위 상태 결정

## 공통 UI 목록

| UI | 설명 | 우선순위 |
| --- | --- | --- |
| Pause Menu | 일시정지 메뉴 | 필수 |
| Option Menu | 볼륨/화면 등 설정 | 필수 |
| Confirm Popup | 종료/포기 확인 | 필수 |
| Loading UI | 씬 전환 중 표시 | 가능하면 포함 |
| Toast/Notice | 짧은 알림 | 선택 |
| Run Result UI | 런 결과 요약 | 가능하면 포함 |

## 입력 모드

| 모드 | 사용 상황 |
| --- | --- |
| Game Only | 전투 연출 중 UI 조작을 막아야 할 때 |
| UI Only | Pause, Option, Confirm Popup |
| Game + UI | 전투 중 카드 선택, 타겟팅, 스테이지 선택 |
| Locked | 씬 전환, 로딩, 필살기 연출 중 |

## Common UI 상태

Common UI 상태는 `CommonUISystem`이 소유한다. Game Manager는 `Paused` 같은 전역 상태만 관리하고, Pause Menu인지 Option Menu인지 Confirm Popup인지는 Common UI가 관리한다.

| 상태 | 설명 |
| --- | --- |
| None | 공통 오버레이 UI 없음 |
| PauseMenu | 일시정지 메뉴 표시 중 |
| OptionMenu | 옵션 메뉴 표시 중 |
| ConfirmPopup | 확인 팝업 표시 중 |
| Loading | 로딩 UI 표시 중 |
| Notice | 알림 또는 경고 표시 중 |

## Widget Stack 규칙

공통 UI는 스택 구조로 관리한다.

```text
Battle HUD
-> Pause Menu
-> Option Menu
-> Confirm Popup
```

닫을 때는 가장 위에 있는 UI부터 닫는다.

```text
Confirm Popup close
-> Option Menu close
-> Pause Menu close
-> Battle HUD active
```

## Pause Menu 항목

MVP 필수:

- Resume
- Options
- Quit to Main Menu

가능하면 포함:

- Restart Run
- Save and Quit

## Option Menu 항목

MVP 필수:

- Master Volume

가능하면 포함:

- BGM Volume
- SFX Volume
- Fullscreen / Windowed
- Resolution

보류:

- 언어 설정
- 키 리바인딩
- 접근성 세부 옵션

## 다른 시스템과의 관계

| 시스템 | 관계 |
| --- | --- |
| Game Manager | Pause/Resume, Option 요청을 받는다 |
| Scene Management | Loading UI, Fade UI 표시 요청을 받는다 |
| Battle UI System | Battle HUD와 공통 UI가 겹치지 않도록 레이어를 분리한다 |
| Save System | Option 값 저장 요청을 보낸다 |
| Input System | 입력 모드 전환 요청을 보낸다 |

## UI Layer 기준

| Layer | 용도 |
| --- | --- |
| Base | Battle HUD, Stage UI, Reward UI |
| Overlay | Pause Menu, Option Menu |
| Modal | Confirm Popup |
| System | Loading UI, Fade UI |
| Notice | Toast, Warning |

## 컴포넌트/클래스 후보

| 이름 | 형태 | 책임 |
| --- | --- | --- |
| UCommonUISubsystem | GameInstance Subsystem | 공통 UI 생성/스택 관리 |
| UInputModeController | UObject 또는 Component | 입력 모드 전환 |
| WBP_PauseMenu | UMG Widget | 일시정지 메뉴 |
| WBP_OptionMenu | UMG Widget | 옵션 메뉴 |
| WBP_ConfirmPopup | UMG Widget | 확인 팝업 |
| WBP_LoadingScreen | UMG Widget | 로딩 화면 |

## 최소 구현 범위

P0:

- Esc로 Pause Menu 열기
- Resume
- Option Menu 열기/닫기
- Quit to Main Menu Confirm Popup
- UI Only / Game + UI 입력 모드 전환

P1:

- Master Volume 조절
- Loading UI
- Run Result UI

P2:

- BGM/SFX 분리 볼륨
- Fullscreen / Windowed
- Toast/Notice

## 테스트 기준

- Battle 상태에서 Esc를 누르면 Pause Menu가 열린다.
- Pause 상태에서 카드 클릭과 턴 종료가 실행되지 않는다.
- Option Menu를 닫으면 이전 UI 상태로 돌아간다.
- Confirm Popup이 열리면 아래 UI가 클릭되지 않는다.
- Loading UI 중 입력이 잠긴다.
- Game Manager가 어떤 위젯을 직접 생성하거나 닫지 않는다.

## 리스크

| 리스크 | 설명 | 대응 |
| --- | --- | --- |
| UI 중첩 꼬임 | Pause와 Option, Confirm이 동시에 꼬일 수 있음 | Widget Stack으로 닫기 순서 통제 |
| 입력 모드 누락 | UI를 닫은 뒤 전투 입력이 복구되지 않을 수 있음 | 이전 입력 모드 저장 |
| Battle UI와 책임 충돌 | Battle HUD까지 Common UI가 처리할 위험 | 전투 전용 UI는 `BattleUISystem.md`로 분리 |
