
#include "Core/ZombieShooterGameMode.h"
#include "Core/ZombieShooterPlayerController.h"

AZombieShooterGameMode::AZombieShooterGameMode()
{
	PlayerControllerClass = AZombieShooterPlayerController::StaticClass();
}
