// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemyCharacter.generated.h"

class AEnemyAIController;
class UBehaviorTree;

UCLASS()
class ZOMBIESHOOTER_API ABaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseEnemyCharacter();
#pragma region Overrides
protected:

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

public:
	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region CoreVariables
public:	
	//AEnemyAIController* MyAIController;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Behavior")
	//TSubclassOf< AEnemyAIController> AIClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Behavior")
	//UBehaviorTree* BehaviorTree;

#pragma endregion



};
