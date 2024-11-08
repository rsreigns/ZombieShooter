// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pawns/Vehicle/Wheels/ZombieShooterWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UZombieShooterWheelRear::UZombieShooterWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}