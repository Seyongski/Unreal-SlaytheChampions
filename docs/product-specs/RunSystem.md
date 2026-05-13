# RunSystem.md

## 목적

이 문서는 런 전체에서 유지되는 상태와 저장/로드 대상, 스테이지 전환 흐름을 정의한다.

RunSystem은 전투 내부 상태를 직접 계산하지 않고, 런 단위의 영속 상태를 관리한다.

## 런 유지 데이터

| 데이터 | 유지 범위 |
| --- | --- |
| 파티 구성 | 런 전체 |
| 파티원 덱 | 런 전체 |
| 골드 | 런 전체 |
| 유물/장비 | 런 전체 |
| 현재 Act | 런 전체 |
| 현재 스테이지/노드 | 런 전체 |
| 카드 보상 결과 | 런 전체 |

## 런 흐름

```text
RunStart
-> 파티/시작 덱 생성
-> StageStart
-> StageGrid 진행
-> Battle/Reward/Event 처리
-> StageClear
-> 다음 Stage 또는 Boss
-> RunClear 또는 RunFail
```

Run 상태는 `RunSystem`이 소유한다. Game Manager는 Run 시작/종료 요청만 보내고, 현재 노드나 보상 단계 같은 세부 진행 상태는 직접 관리하지 않는다.

## Run 상태

| 상태 | 설명 |
| --- | --- |
| None | Run이 시작되지 않음 |
| RunInit | 파티, 시작 덱, 초기 골드, 시작 노드 준비 |
| StageSelect | 스테이지 그리드에서 다음 노드 선택 |
| BattleNode | 전투 노드 처리 중 |
| RewardNode | 전투 보상 처리 중 |
| EventNode | 이벤트 노드 처리 중 |
| RestNode | 휴식 노드 처리 중 |
| ShopNode | 상점 노드 처리 중 |
| StageClear | 스테이지 클리어 정산 |
| RunClear | Run 성공 |
| RunFail | Run 실패 |

## 저장 대상

MVP에서 저장/로드는 보류 가능하지만, 데이터 구조는 저장 가능한 형태로 둔다.

| 항목 | 저장 필요 |
| --- | --- |
| 파티원 목록 | 필요 |
| 파티원별 덱 | 필요 |
| 골드 | 필요 |
| 유물 | 필요 |
| 현재 노드 | 선택 |
| 전투 중 손패 | MVP 보류 |

## 연결 시스템

| 시스템 | 연결 |
| --- | --- |
| `StageGridSystem.md` | 스테이지 진행 |
| `RewardSystem.md` | 보상 결과 반영 |
| `CharacterSystem.md` | 파티 구성 |
| `CardSystem.md` | 덱 변화 |
| `GameManager.md` | Run 시작/종료와 상위 상태 전환 요청 |
| `SceneManagement.md` | Run 상태에 따른 씬 전환 |

## 테스트 기준

- 보상이 런 상태에 반영되는가?
- 덱 변화가 다음 전투까지 유지되는가?
- 스테이지 클리어 후 Stage Scope 상태가 사라지는가?
- 런 전체 데이터와 전투 내부 데이터가 섞이지 않는가?
- Game Manager가 현재 노드 상태를 직접 관리하지 않는가?

## 미정 항목

- 저장/로드를 1차 마스터에 포함할지 여부
- 런 실패 시 보상 또는 해금이 있는지
- Act 수와 스테이지 수
