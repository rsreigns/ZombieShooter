// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Templates/SharedPointer.h"
#include "EnvironmentQuery/EnvQueryTypes.h" 
#include "EnvironmentQuery/Items/EnvQueryItemType.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AIManagerBase.generated.h"

class ABaseEnemyCharacter;
class AStaticAIVolume;
class AStaticVolumeSpawner;
class UEnvQuery;

struct FTimerHandle;
struct FEnvQueryResult;

UCLASS()
class ZOMBIESHOOTER_API AAIManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	

	AAIManagerBase();
#pragma region Overrides

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

#pragma endregion


#pragma region CoreVariables
	UPROPERTY(BlueprintReadOnly, Category = "AI|Core|Pool")
	TArray<ABaseEnemyCharacter*> StaticPool;


	//TArray<TWeakObjectPtr<ABaseEnemyCharacter>> StaticPool;

	//UPROPERTY(BlueprintReadOnly, Category = "AI|Core|Pool")
	//TArray<ABaseEnemyCharacter*> DynamicPool;
	//UPROPERTY(BlueprintReadOnly, Category = "AI|Core|Pool")
	//TArray<ABaseEnemyCharacter*> HotspotPool;

	// Default variables that can be tweaked

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|Pool")
	int32 StaticPoolSize = 12;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|Pool")
	//int32 DynamicPoolSize = 100;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|Pool")
	//int32 HotspotPoolSize = 50;

	// EQS To use for each category
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|EQS")
	//UEnvQuery* StaticEQS;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|EQS")
	//UEnvQuery* DynamicEQS;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core|EQS")
	//UEnvQuery* HotspotEQS;

	// Classes to be assigned in BP

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core")
	TSubclassOf<ABaseEnemyCharacter> AIToSpawn;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core")
	//TSubclassOf<AStaticAIVolume> StaticVolumeClass;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI|Core")
	//TSubclassOf<AStaticVolumeSpawner> StaticVolumeSpawnerClass;

	//UPROPERTY(BlueprintReadOnly, Category = "AI|Core")
	//TArray<AStaticAIVolume*> SpawnedVolumes;

	// Global variables

	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> SpawnerActors;

	//int32 LastUsedVolumeIndex; // stores last volume's index to find it when volume changes
	//int32 CurrentVolumeIndex; // to track current volume and assign it with AI's

	//FVector FetchedLocation;


	FTimerHandle CheckTimer;

#pragma endregion


#pragma region CoreFunctions

	void SpawnEnemyPool();
	UFUNCTION(BlueprintCallable)
	void ApplyPoolObjectDefaults(bool bApply ,ABaseEnemyCharacter* PoolObject);
	UFUNCTION(BlueprintCallable)
	void TakeIntoPool(ABaseEnemyCharacter* AI);
	UFUNCTION(BlueprintCallable)
	ABaseEnemyCharacter* GetAIFromPool();
	void CheckSpawnedAI();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnAI();
	//void GetSpawnVolumes();
	//void OnNotifyVolumeBeginOverlap(int32 InIndex);
	//void OnNotifyVolumeEndOverlap(int32 OutIndex);
	//void RunEQS(FVector AtLocation, UEnvQuery* Query);
	//void FetchEQSLocation(TSharedPtr<FEnvQueryResult> Result);

#pragma endregion



#pragma region Getters
#pragma endregion
};
