#include "Map/DebugRunInputActor.h"

#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Map/RunSystem.h"
#include "Relic/RelicSubsystem.h"

ADebugRunInputActor::ADebugRunInputActor()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void ADebugRunInputActor::BeginPlay()
{
	Super::BeginPlay();

	//디버그용
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		EnableInput(PlayerController);
		if (InputComponent)
		{
			InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ADebugRunInputActor::DebugReturnToMap);
			InputComponent->BindKey(EKeys::Six, IE_Pressed, this, &ADebugRunInputActor::DebugLogInitializedRelics);
			InputComponent->BindKey(EKeys::NumPadSix, IE_Pressed, this, &ADebugRunInputActor::DebugLogInitializedRelics);
		}
	}
}

void ADebugRunInputActor::DebugReturnToMap()
{
	//디버그용
	if (!GetWorld())
	{
		return;
	}

	const FString CurrentMapName = GetWorld()->RemovePIEPrefix(GetWorld()->GetMapName());
	if (CurrentMapName == ReturnLevelName.ToString())
	{
		return;
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		//디버그용
		if (URunSystem* RunSystem = GameInstance->GetSubsystem<URunSystem>())
		{
			RunSystem->AreaCleared();
		}
	}
}

void ADebugRunInputActor::DebugLogInitializedRelics()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		if (URelicSubsystem* RelicSubsystem = GameInstance->GetSubsystem<URelicSubsystem>())
		{
			RelicSubsystem->LogInitializedRelics();
		}
	}
}
