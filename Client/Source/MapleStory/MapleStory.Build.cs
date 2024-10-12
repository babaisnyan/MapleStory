using UnrealBuildTool;

public class MapleStory : ModuleRules
{
	public MapleStory(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "Paper2D",
			"GameplayAbilities", "GameplayTags", "GameplayTasks", "Json", "JsonUtilities",
			"EnhancedInput", "UMG", "Slate", "SlateCore", "PaperZD",
			"Sockets", "Networking", "TcpSocketPlugin"
		});

		PrivateDependencyModuleNames.AddRange(new[] {"ProtobufCore", "PaperFlipbookWidget"});

		PrivateIncludePaths.AddRange(new[] {"MapleStory/Public", "MapleStory/Public/Network/Protocols/"});
	}
}