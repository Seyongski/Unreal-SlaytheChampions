# collaboration-workflow.md

## 목적

이 문서는 프로젝트 협업 시 Notion과 GitHub를 어떻게 나누어 사용할지 정의한다.

Notion은 기획, 일정, 회의, 작업 일지를 관리한다. GitHub는 구현 단위, 이슈, PR, 커밋 기록을 관리한다.

## Notion 사용 기준

### 협업 작업 보드

사용 위치:

- `Unreal Slay the Champions - 협업 작업 보드`

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

- `Unreal Slay the Champions - 데일리 로그`

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

## 현재 자동화 상태

- Notion 작업 보드, 타임라인, 데일리 로그는 생성 완료.
- GitHub Issue/PR 템플릿은 레포에 추가 완료.
- GitHub 원격 Issue 자동 생성은 현재 GitHub 앱 권한 부족으로 보류.
- GitHub Projects 자동 생성은 `gh` CLI 설치와 인증 후 진행한다.
