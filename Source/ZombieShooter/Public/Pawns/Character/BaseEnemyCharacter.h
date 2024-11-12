// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pawns/Character/BaseCharacter.h"
#include "BaseEnemyCharacter.generated.h"

class AEnemyAIController;
class UBehaviorTree;
class AAIManagerBase;

UCLASS()
class ZOMBIESHOOTER_API ABaseEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemyCharacter();
#pragma region  Components

#pragma endregion

#pragma region Overrides
protected:

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


public:
	virtual void Tick(float DeltaTime) override;

#pragma endregion

#pragma region CoreVariables
public:	
	bool bIsDead = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Crowd")
	int32 BaseWalkSpeed = 75;
	UPROPERTY(BlueprintReadOnly)
	bool bIsInPool;

	UPROPERTY()
	AAIManagerBase* Manager;

#pragma endregion
#pragma region CoreFunctions
	virtual void OnDeathEvent() override;
	UFUNCTION()
	void SendToPool();

#pragma endregion


};
