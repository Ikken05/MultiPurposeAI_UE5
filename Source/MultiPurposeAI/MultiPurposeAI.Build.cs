// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MultiPurposeAI : ModuleRules
{
	public MultiPurposeAI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
