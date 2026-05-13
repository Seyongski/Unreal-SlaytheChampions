# SceneManagement.md

## Scene Management System

## 목적

Scene Management System은 레벨, 씬, 화면 전환을 담당한다.

Game Manager가 `어떤 상태로 가야 하는지`를 결정한다면, Scene Management는 `그 상태를 보여주기 위해 어떤 레벨이나 씬을 열고 닫을지`를 처리한다.

## 핵심 책임

- Main Menu, Stage, Battle, Reward, GameOver 화면 전환
- Start Scene / End Scene 연결
- Persistent Level 사용 여부 관리
- Level Load / Unload 요청 처리
- 전환 중 입력 잠금 요청
- Loading 화면 표시 요청
- 씬 전환 완료 이벤트 발행

## 직접 처리하지 않는 것

- 게임 상위 상태 결정
- 카드/전투/보상 로직
- Pause Menu, Option Menu 내부 UI
- 개별 위젯 표시 데이터
- Run 데이터 저장

## Scene 목록

| Scene | 설명 | 우선순위 |
| --- | --- | --- |
| MainMenu | 메인 메뉴 | 필수 |
| Stage | 스테이지 그리드 선택 | 필수 |
| Battle | 전투 화면 | 필수 |
| Reward | 전투 보상 화면 | 필수 |
| GameOver | 패배 또는 Run 종료 화면 | 필수 |
| StartScene | 시작 연출 | 선택 |
| EndScene | 엔딩 연출 | 선택 |
| Loading | 씬 전환 중 표시 | 가능하면 포함 |

## 권장 전환 흐름

```text
Game Manager
-> RequestScene(Stage)
-> Scene Management locks input
-> Common UI opens Loading UI
-> Scene Management loads Stage scene
-> Scene Management unlocks input
-> Common UI closes Loading UI
-> SceneLoaded event
```

## Persistent Level 기준

MVP에서는 복잡한 스트리밍 구조보다 단순한 전환을 우선한다.

권장:

- MainMenu는 별도 레벨
- Stage/Battle/Reward는 하나의 Gameplay Persistent Level 안에서 UI/Actor 상태를 전환
- StartScene/EndScene은 시간이 남을 때 별도 레벨 또는 시퀀스로 추가

보류:

- 복잡한 World Partition
- 대규모 Level Streaming
- 다중 Act용 복합 로딩 구조

## 전환 상태

| 상태 | 설명 |
| --- | --- |
| Idle | 전환 없음 |
| Preparing | 전환 요청 검증 |
| Loading | 레벨 또는 씬 로딩 중 |
| Applying | 로딩 후 초기 상태 적용 |
| Complete | 전환 완료 |
| Failed | 전환 실패 |

## 다른 시스템과의 관계

| 시스템 | 관계 |
| --- | --- |
| Game Manager | 씬 전환 요청을 보낸다 |
| Common UI System | Loading UI, Fade UI, Confirm UI 표시 요청을 받는다 |
| Run System | Run 상태에 따라 진입할 Scene 정보를 제공한다 |
| Combat Kernel | Battle Scene 준비 완료 후 전투 시작 이벤트를 받는다 |
| Stage Grid System | Stage Scene 준비 완료 후 그리드 표시를 시작한다 |

## 컴포넌트/클래스 후보

| 이름 | 형태 | 책임 |
| --- | --- | --- |
| USceneFlowSubsystem | GameInstance Subsystem | Scene 전환 요청 처리 |
| FSceneTransitionRequest | Struct | 전환 대상과 옵션 데이터 |
| ESceneId | Enum | MainMenu, Stage, Battle, Reward 등 씬 식별자 |
| WBP_LoadingScreen | UMG Widget | 로딩 화면 |

## 최소 구현 범위

P0:

- MainMenu -> Gameplay 진입
- Gameplay 내부에서 Stage/Battle/Reward 상태 전환 지원
- GameOver 화면 진입
- 전환 중 입력 잠금

P1:

- Loading UI 표시
- Quit to Main Menu 전환
- 실패 시 fallback 처리

P2:

- Start Scene
- End Scene
- 간단한 Fade In/Out

## 테스트 기준

- MainMenu에서 Start 요청 시 Gameplay 화면으로 전환된다.
- Battle 종료 후 Reward 화면으로 전환된다.
- Quit to Main Menu 요청 시 MainMenu로 돌아간다.
- 전환 중 카드 클릭이나 턴 종료 입력이 실행되지 않는다.
- Loading UI가 열리고 전환 완료 후 닫힌다.

## 리스크

| 리스크 | 설명 | 대응 |
| --- | --- | --- |
| 레벨 구조 과복잡화 | 초반부터 스트리밍 구조가 커질 수 있음 | MVP는 단순 Gameplay Level 중심 |
| 전환 중 입력 실행 | 로딩 중 입력이 살아 있으면 상태가 꼬일 수 있음 | Input Lock과 Loading 상태 도입 |
| Start/End Scene 일정 부담 | 연출 제작 시간이 부족할 수 있음 | Phase 5 선택 범위로 유지 |
