// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;

UCLASS()
class ZOMBIESHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
	
#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Components")
	UAIPerceptionComponent* PerceptionComp;

#pragma endregion

#pragma region Functions

	void OnPerceptionUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

#pragma endregion
};
