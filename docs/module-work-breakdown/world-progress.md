# World Progress

## 역할

전투 외부의 로그라이크 흐름과 한 판의 진행 상태를 담당한다.

## 담당 범위

- Grid Map
- Run Manager
- Persistence
- 골드, 유물, 현재 층수
- 스테이지 클리어 시 초기화/유지 값 구분
- 전투 보상 흐름과 다음 노드 연결

## 하위 시스템

| 하위 시스템 | 내용 | 완료 기준 |
| --- | --- | --- |
| Grid Map | 전투, 휴식, 상점, 이벤트, 엘리트, 보스 노드 생성과 경로 선택 | 플레이어가 다음 노드를 선택할 수 있다 |
| Run Manager | 골드, 유물, 현재 층수, 파티 상태 등 Run 데이터 관리 | 전투 밖에서도 런 상태가 유지된다 |
| Persistence | 초기화될 값과 유지될 값을 구분한다 | 스테이지 클리어 시 필살기 조건은 초기화되고 덱 상태는 유지된다 |

## Phase별 목표

| Phase | 목표 |
| --- | --- |
| Phase 1 | 전투 단위 시작/종료만 처리 |
| Phase 2 | 반복 전투 테스트용 임시 런 상태 |
| Phase 3 | Grid Map, 보상, 상태 유지/초기화 규칙 구현 |
| Phase 4 | Act 1 전체 흐름, 상점/휴식/이벤트 연결 |
| Phase 5 | 마스터 빌드 진행 안정성 점검 |

## 참조 문서

- `docs/product-specs/StageGridSystem.md`
- `docs/product-specs/RunSystem.md`
- `docs/product-specs/RewardSystem.md`
- `docs/mechanics-harness/level-rules.md`
