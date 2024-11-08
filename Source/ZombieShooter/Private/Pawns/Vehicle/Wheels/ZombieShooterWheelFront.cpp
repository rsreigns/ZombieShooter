// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pawns/Vehicle/Wheels/ZombieShooterWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UZombieShooterWheelFront::UZombieShooterWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}