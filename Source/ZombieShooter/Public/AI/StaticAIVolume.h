// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticAIVolume.generated.h"


class USphereComponent;
class AMyCharacter;
class ABaseEnemyCharacter;
class AAIManagerBase;

UCLASS()
class ZOMBIESHOOTER_API AStaticAIVolume : public AActor
{
	GENERATED_BODY()
	
public:	

	AStaticAIVolume();
#pragma region Overrides
protected:

	virtual void BeginPlay() override;
public:	

	virtual void Tick(float DeltaTime) override;
#pragma endregion

#pragma region CoreComponents

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core")
	USphereComponent* SphereOverlap;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core")
	float SpawnRate = 2.f;
#pragma endregion

#pragma region CoreVariables

	TObjectPtr<AMyCharacter> MyCharacter;
	UPROPERTY(BlueprintReadOnly, Category = "AI|Core")
	int32 Index;
	int32 CurrentSpawnedAI = 0;
	TArray<ABaseEnemyCharacter*> StaticAIPawns;
	TObjectPtr<AAIManagerBase> AIManagerRef;
	FTimerHandle SpawnHandle;
#pragma endregion

#pragma region CoreFunctions

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion




};
