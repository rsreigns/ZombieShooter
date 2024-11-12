// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAIPerceptionComponent;
class UAISense;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;
class UBehaviorTree;

UCLASS()
class ZOMBIESHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController(const FObjectInitializer& ObjectInitializer);
	
#pragma region Components

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Components")
	UAIPerceptionComponent* PerceptionComp;

#pragma endregion

#pragma region Configs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Config")
	UAISenseConfig_Sight* SightConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Config")
	UAISenseConfig_Hearing* HearConfig;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Config")
	UAISenseConfig_Damage* DamageConfig;

#pragma endregion

#pragma region Overrides

	virtual void BeginPlay() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
#pragma endregion

#pragma region Variables

	UPROPERTY(EditDefaultsOnly, meta = (UIMin = "1", UIMax = "4"), Category = "AI|Crowd")
	int32 AvoidanceQuality = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Crowd")
	float CollisionQueryRange = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Behavior")
	UBehaviorTree* BTAsset;

	
#pragma endregion

#pragma region Functions
	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

#pragma endregion

};
