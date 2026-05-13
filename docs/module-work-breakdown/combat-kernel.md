# Combat Kernel

## 역할

전술 전투 시스템의 핵심 규칙을 담당한다. 카드 입력을 행동 이벤트로 변환하고, 턴 종료 시 정해진 순서대로 실행하며, 전투의 상태 전환과 상위 판정을 관리한다.

## 담당 범위

- Action Queue FIFO 실행 엔진
- Targeting Logic
- Turn Manager
- Joint / Ultimate 상위 판정
- 전투 승리/패배 판정
- 전투 로그와 디버그 출력

## 하위 시스템

| 하위 시스템 | 내용 | 완료 기준 |
| --- | --- | --- |
| Action Queue | 카드를 선택한 순서대로 Action Event를 쌓고 `InputIndex`를 저장한다 | 턴 종료 시 먼저 선택한 카드부터 실행된다 |
| Targeting Logic | Self, Target, All, Field 등 카드 데이터에 따른 대상 탐색 | 카드 대상 타입만 바꿔도 대상 탐색이 달라진다 |
| Turn Manager | 플레이어 입력 턴, 플레이어 실행 턴, 적 AI 턴, 효과 정산 상태 제어 | 한 턴 흐름이 끊기지 않고 반복된다 |
| Joint / Ultimate | 입력 순서 조합과 필살기 조건을 추적한다 | 합동기/필살기가 조건 충족 시 자동 판정된다 |

## Phase별 목표

| Phase | 목표 |
| --- | --- |
| Phase 1 | Action Queue, Turn Manager, 단일 대상 공격/방어 실행 |
| Phase 2 | 합동기, 필살기 자동 승격, 다중 대상 처리 |
| Phase 3 | 스테이지 내 필살기 조건 유지/초기화 연결 |
| Phase 4 | 보스 기믹과 전투 규칙 연결 |
| Phase 5 | 로그, 밸런스 검증, 연출 트리거 정리 |

## 참조 문서

- `docs/product-specs/CombatSystem.md`
- `docs/product-specs/ActionQueueSystem.md`
- `docs/product-specs/TargetResolverSystem.md`
- `docs/product-specs/UltimateComboSystem.md`
