# collaboration-workflow.md

## 목적

이 문서는 프로젝트 협업 시 Notion과 GitHub를 어떻게 나누어 사용할지 정의한다.

Notion은 기획, 일정, 회의, 작업 일지를 관리한다. GitHub는 구현 단위, 이슈, PR, 커밋 기록을 관리한다.

## Notion 사용 기준

### 협업 작업 보드

사용 위치:

- Notion: `Unreal Slay the Champions - 협업 작업 보드`
- URL: https://www.notion.so/72778adcab2a437bbc44dd344e828151

용도:

- 칸반 보드로 작업 상태를 공유한다.
- 타임라인 뷰로 Phase별 작업 일정을 확인한다.
- 기획, 디자인, 개발, QA, 문서 작업을 한 곳에서 본다.

상태 기준:

| 상태 | 의미 |
| --- | --- |
| 할 일 | 아직 착수하지 않은 작업 |
| 진행 중 | 현재 진행 중인 작업 |
| 피드백 대기 | 리뷰, 확인, 결정이 필요한 작업 |
| 완료 | 완료된 작업 |
| 보류 | 마감 또는 선행 조건 때문에 미룬 작업 |

### 데일리 로그

사용 위치:

- Notion: `Unreal Slay the Champions - 데일리 로그`
- URL: https://www.notion.so/20a24294606843b2831f8749a021d9df

매일 퇴근 전 아래 항목만 짧게 남긴다.

- 오늘 한 작업
- 오늘 고민한 지점
- 결정/변경 사항
- 관련 링크
- 다음 액션

## GitHub 사용 기준

### Issues

코드, 로직, 구현 단위로 추적해야 하는 작업은 GitHub Issue로 등록한다.

Issue 템플릿:

- `Planning Task`: 기획, 문서, 밸런스, UI/UX 설계
- `Dev Task`: 구현 작업
- `Bug Report`: 버그와 플레이 테스트 문제
- `Daily Log`: GitHub에도 남겨야 하는 일일 작업 요약

권장 라벨:

- `planning`
- `dev`
- `design`
- `bug`
- `daily-log`
- `phase-1`
- `phase-2`
- `feedback-needed`
- `tech-debt`

라벨 기준은 `.github/labels.yml`에 기록한다. GitHub 앱 권한이나 `gh` CLI 인증이 준비되면 원격 라벨로 생성한다.

현재 원격 라벨은 `gh` CLI로 생성 완료했다.

### Module-based Issue 운영

팀 작업은 Phase 확장 순서보다 모듈 오너십을 우선해서 관리한다.

Phase는 일정과 마감 기준이고, Issue의 실제 담당 단위는 아래 모듈을 기준으로 한다.

- `Combat Kernel`
- `Card & Deck`
- `Entity & Stats`
- `NPC Brain`
- `World Progress`
- `Interface & Camera`

모듈별 상세 작업 범위는 `docs/module-work-breakdown.md`와 `docs/module-work-breakdown/` 폴더의 개별 문서를 따른다.

Issue 제목은 아래 형식을 권장한다.

```text
[Module][Phase] 작업명
```

예시:

```text
[Combat Kernel][Phase 1] Action Queue FIFO 실행 구현
[Card & Deck][Phase 1] 캐릭터별 덱/손패/버림 pile 구현
[Entity & Stats][Phase 2] 상태이상 Tick 처리 구현
[NPC Brain][Phase 4] 보스 기믹 패턴 구현
[World Progress][Phase 3] Grid Map 노드 선택 구현
[Interface & Camera][Phase 1] 전투 HUD와 파티원 선택 UI 구현
```

Issue 본문에는 최소한 아래 항목을 포함한다.

- 목표: 이 작업이 해결해야 하는 문제
- 모듈: 담당 모듈
- Phase: 적용되는 일정 단계
- 관련 문서: `docs/product-specs/`, `docs/module-work-breakdown/` 링크
- 완료 기준: 테스트하거나 확인할 수 있는 체크리스트
- 리스크: 다른 모듈과 충돌할 수 있는 지점

Issue 라벨은 작업 성격과 Phase를 함께 붙인다.

| 상황 | 권장 라벨 |
| --- | --- |
| 구현 작업 | `dev`, `phase-n` |
| 기획/문서 작업 | `planning`, `phase-n` |
| UI/UX/카메라 작업 | `design`, `phase-n` |
| 회의 또는 결정 필요 | `meeting-needed`, `feedback-needed`, `phase-n` |
| 기술 부채 또는 후순위 이관 | `tech-debt`, `phase-n` |

Issue를 만들기 전에 Notion 작업 보드에 같은 이름의 작업을 먼저 만들고, GitHub Issue 생성 후 Notion의 `GitHub 링크` 필드에 Issue URL을 붙인다.

PR을 만들 때는 관련 Issue 번호를 PR 본문에 연결한다.

```text
Related Issue: #12
```

작업이 완료되면 GitHub Issue를 닫고, Notion 작업 상태도 수동으로 `완료`로 변경한다.

### Projects

GitHub Project는 구현 이슈의 개발 진행 상태를 보는 보드로 사용한다.

사용 위치:

- GitHub Project: `Unreal Slay the Champions - Phase 1 Board`
- URL: https://github.com/users/Muru24/projects/2


### Pull Requests

PR은 실제 변경 묶음을 설명한다.

PR 본문에는 아래 항목을 반드시 적는다.

- 요약
- 변경 사항
- 관련 문서 / 이슈
- 검증
- 리스크 / 후속 작업

## 권장 흐름

```text
Notion 작업 보드에서 작업 생성
-> 구현이 필요한 작업은 GitHub Issue 생성
-> Notion 작업 카드의 GitHub 링크 칸에 Issue URL 수동 연결
-> 브랜치 작업
-> PR 생성
-> PR에 Notion/Issue 링크 연결
-> 머지 후 Notion 작업 상태 완료로 변경
-> 데일리 로그에 결정/변경 기록
```

## 운영 규칙

1. 기획 고민과 결정은 Notion에 먼저 남긴다.
2. 구현이 필요한 항목은 GitHub Issue로 옮긴다.
3. PR은 코드 변경뿐 아니라 관련 하네스 문서 변경 여부를 확인한다.
4. 새 리스크가 생기면 `tech-debt-tracker.md`에도 기록한다.
5. 매일 작업 종료 전 데일리 로그를 남긴다.
6. Notion과 GitHub 상태는 자동 동기화하지 않고, 작업자가 수동으로 맞춘다.
