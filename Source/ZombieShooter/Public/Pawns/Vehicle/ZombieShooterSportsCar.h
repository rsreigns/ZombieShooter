// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ZombieShooterPawn.h"
#include "ZombieShooterSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class ZOMBIESHOOTER_API AZombieShooterSportsCar : public AZombieShooterPawn
{
	GENERATED_BODY()
	
public:

	AZombieShooterSportsCar();
};
