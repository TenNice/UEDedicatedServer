// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DediTest : ModuleRules
{
	public DediTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Json", "JsonUtilities", "HTTP", "UMG", "Networking", "Sockets", "UniversalVoiceChatPro", "Voice", "OnlineSubsystem", "OnlineSubsystemUtils" });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}
