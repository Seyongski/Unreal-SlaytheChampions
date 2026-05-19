// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SlayTheChampions : ModuleRules
{
	public SlayTheChampions(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			 "UMG",        // UserWidget, UImage, UTextBlock
			"Slate",      // 추가
			"SlateCore",
            "Paper2D"     // UPaperSprite 사용을 위해 추가
		});


		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
