// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZombieShooter : ModuleRules
{
	public ZombieShooter(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{ "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "UMG" , "MotionWarping",

            "EnhancedInput", "ChaosVehicles", "PhysicsCore" });
	}
}
