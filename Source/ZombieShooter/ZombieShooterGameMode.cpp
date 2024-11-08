// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieShooterGameMode.h"
#include "ZombieShooterPlayerController.h"

AZombieShooterGameMode::AZombieShooterGameMode()
{
	PlayerControllerClass = AZombieShooterPlayerController::StaticClass();
}
