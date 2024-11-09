// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API AMyPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AMyPlayerController();
protected:
	FGenericTeamId TeamID;
public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	
};
