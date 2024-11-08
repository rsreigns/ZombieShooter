// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieShooterWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UZombieShooterWheelFront::UZombieShooterWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}