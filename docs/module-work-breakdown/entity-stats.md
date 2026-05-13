# Entity & Stats

## 역할

플레이어와 적의 공통 부모 구조, 스탯 처리, 상태이상, 직업 고유 메커니즘을 담당한다.

## 담당 범위

- Stat Component
- Effect System
- Job System
- HP, Block, Defense Up, 공격력/방어력 증감
- 상태이상 Tick 처리와 중첩
- 검사, 마법사, 힐러 직업 고유 메커니즘

## 하위 시스템

| 하위 시스템 | 내용 | 완료 기준 |
| --- | --- | --- |
| Stat Component | HP, Block, 코스트 관련 참조, 공격력/방어력 증감 관리 | 피해 계산에 Block과 Defense Up이 반영된다 |
| Effect System | 상태이상 4~6종의 Tick 처리, 지속 턴, 중첩 계산 | 턴 종료/시작 시 상태이상이 규칙대로 처리된다 |
| Job System | 검사, 마법사, 힐러의 고유 메커니즘과 필살기 조건 | 직업별 카드와 필살기 조건이 분리된다 |

## Phase별 목표

| Phase | 목표 |
| --- | --- |
| Phase 1 | 플레이어/적 공통 체력, Block, Defense Up 처리 |
| Phase 2 | 3직업 역할, 상태이상 4~6종, 직업별 조건 추적 |
| Phase 3 | 스테이지 내 유지되는 스탯과 초기화되는 스탯 구분 |
| Phase 4 | 유물, 보상, 보스 기믹이 스탯에 주는 영향 연결 |
| Phase 5 | 직업별 밸런스와 상태이상 과부하 점검 |

## 참조 문서

- `docs/product-specs/CharacterSystem.md`
- `docs/product-specs/StatusEffectSystem.md`
- `docs/product-specs/EffectResolverSystem.md`
- `docs/mechanics-harness/damage-logic.md`
- `docs/mechanics-harness/status-effects.md`
