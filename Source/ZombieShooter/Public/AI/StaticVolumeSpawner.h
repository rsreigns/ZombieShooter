// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticVolumeSpawner.generated.h"


class UCapsuleComponent;

UCLASS()
class ZOMBIESHOOTER_API AStaticVolumeSpawner : public AActor
{
	GENERATED_BODY()
	
public:	

	AStaticVolumeSpawner(); 
#pragma region Overrides
protected:

	virtual void BeginPlay() override;
public:	

	//virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region CoreComponents

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Spawn")
	UCapsuleComponent* DebugCapsule;


#pragma endregion

#pragma region CoreVariables



#pragma endregion

#pragma region CoreFunctions


#pragma endregion




};
