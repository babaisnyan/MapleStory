// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MapleStory : ModuleRules
{
	public MapleStory(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new[] {"Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "GameplayAbilities", "GameplayTags", "GameplayTasks", "Json", "JsonUtilities", "EnhancedInput", "UMG", "Slate", "SlateCore", "PaperZD"});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}