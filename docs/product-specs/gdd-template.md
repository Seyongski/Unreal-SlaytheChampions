# gdd-template.md

## 목적

이 문서는 프로젝트의 코드 작성 기본 원칙을 정의한다.

카드, 캐릭터, 몬스터, 필살기 같은 개별 시스템 기획은 이 파일에 직접 누적하지 않는다. 세부 시스템 문서는 같은 폴더 아래에 별도 파일로 분리한다.

예시:

- `CardSystem.md`
- `CharacterSystem.md`
- `EnemySystem.md`
- `UltimateSystem.md`
- `StageGridSystem.md`

## 기본 구현 원칙

1. 시스템은 가능한 한 컴포넌트 형식으로 만든다.
2. 특정 액터에만 붙는 구조를 피하고, 다른 액터에서도 재사용 가능하게 설계한다.
3. 클래스 간 직접 참조를 줄이고, 데이터와 이벤트를 통해 연결한다.
4. 테스트가 쉽도록 각 클래스의 책임을 작게 유지한다.
5. 전투 규칙과 UI/연출을 분리한다.
6. 카드, 직업, 적, 상태이상, 보상은 데이터 기반으로 확장한다.
7. Blueprint는 빠른 연결과 연출에 쓰고, 핵심 판정 로직은 안정적인 코어 구조로 둔다.

## 컴포넌트 기반 설계

액터는 기능을 직접 많이 들고 있기보다 필요한 컴포넌트를 조합해서 동작한다.

권장 컴포넌트 예시:

| 컴포넌트 | 역할 |
| --- | --- |
| DeckComponent | 덱, 손패, 드로우 pile, discard pile 관리 |
| CardPlayableComponent | 카드 사용 가능 여부와 사용 요청 처리 |
| CombatStatsComponent | HP, Block, Power 등 전투 수치 관리 |
| StatusComponent | 버프, 디버프, 필살기 조건 스택 관리 |
| TargetableComponent | 대상 선택 가능 여부와 진영 정보 제공 |
| ActionQueueComponent | Action Event 등록과 실행 |
| AIIntentComponent | 적 의도와 행동 패턴 관리 |

## 결합도 규칙

피해야 할 구조:

```text
Card -> PlayerCharacter -> BattleManager -> EnemyActor -> UIWidget
```

권장 구조:

```text
CardData
-> ActionEvent
-> EffectResolver
-> Target/Status/Stats Component
-> Result Event
-> UI listens
```

규칙:

- 카드는 액터를 직접 수정하지 않고 Action Event를 만든다.
- Effect Resolver가 실제 수치 변화를 처리한다.
- UI는 전투 상태를 직접 바꾸지 않고 요청만 보낸다.
- 상태 변경 결과는 이벤트나 View Model 형태로 UI에 전달한다.
- 특정 캐릭터 전용 로직도 가능하면 태그와 데이터 조건으로 표현한다.

## 테스트 용이성 원칙

테스트가 쉬운 구조를 우선한다.

확인해야 할 단위:

- 카드 1장이 올바른 Action Event를 생성하는가?
- Action Queue가 사용 순서대로 실행되는가?
- Effect Resolver가 피해/방어/회복을 정확히 처리하는가?
- StatusComponent가 필살기 조건 스택을 스테이지 범위로 유지하는가?
- Ultimate 조건이 이벤트 실행 시 자동 승격되는가?
- 대상이 사망했을 때 큐가 멈추지 않는가?

테스트를 어렵게 만드는 요소:

- UI 위젯에서 전투 규칙을 직접 처리
- 카드 블루프린트마다 서로 다른 하드코딩
- Battle Manager가 모든 세부 효과를 직접 처리
- 액터 간 순환 참조
- 상태 스택과 일반 버프가 같은 변수에 섞이는 구조

## 문서 분리 규칙

세부 설계 문서는 아래 기준으로 분리한다.

| 문서 | 내용 |
| --- | --- |
| CardSystem.md | 카드 데이터, 등급, 코스트, 효과, 보상 |
| CharacterSystem.md | 직업, 파티원, 개별 덱, 성장 |
| EnemySystem.md | 몬스터, 보스, AI 의도, 기믹 |
| UltimateSystem.md | 필살기 자동 승격, 조건 스택, 연출 |
| StageGridSystem.md | 스테이지 그리드, 노드, 보상 흐름 |

## 코드 작성 체크리스트

- 이 기능은 컴포넌트로 분리 가능한가?
- 다른 액터에서도 재사용 가능한가?
- UI 없이도 테스트 가능한가?
- 특정 직업 이름을 하드코딩하지 않았는가?
- 데이터 추가만으로 새 카드/상태/적을 만들 수 있는가?
- 클래스 간 직접 참조가 과도하지 않은가?
- 실패했을 때 디버그 로그로 원인을 알 수 있는가?

