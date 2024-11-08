// Copyright Epic Games, Inc. All Rights Reserved.


#include "Core/ZombieShooterPlayerController.h"
#include "Pawns/Vehicle/ZombieShooterPawn.h"
#include "UI/ZombieShooterUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"

void AZombieShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// spawn the UI widget and add it to the viewport
}

void AZombieShooterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	

}

void AZombieShooterPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);


}

void AZombieShooterPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	VehiclePawn = CastChecked<AZombieShooterPawn>(InPawn);
}
