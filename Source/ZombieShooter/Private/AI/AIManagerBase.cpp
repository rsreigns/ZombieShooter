
// Dependencies
#include "AI/AIManagerBase.h"
#include "Kismet/GameplayStatics.h"

// EQS
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/Items/EnvQueryItemType.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AI/EQS/Custom_EnvQueryContext.h"
// Classes
#include "Pawns/Character/BaseEnemyCharacter.h"
#include "AI/StaticAIVolume.h"
#include "AI/StaticVolumeSpawner.h"

//Debug
#include "DebugHelper.h"

AAIManagerBase::AAIManagerBase()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void AAIManagerBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemyPool();
	GetSpawnVolumes();
	
}

void AAIManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIManagerBase::SpawnEnemyPool()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform Transform = GetActorTransform();
	for (int i = 0; i < StaticPoolSize + DynamicPoolSize + HotspotPoolSize; i++)
	{
		ABaseEnemyCharacter* EnemyAI = GetWorld()->SpawnActor<ABaseEnemyCharacter>(AIToSpawn, Transform, SpawnParams);
		if (StaticPool.Num() <= StaticPoolSize)
		{
			StaticPool.Push(EnemyAI);
		}
		if (DynamicPool.Num() <= DynamicPoolSize)
		{
			DynamicPool.Push(EnemyAI);
		}
		if (HotspotPool.Num() <= HotspotPoolSize)
		{
			HotspotPool.Push(EnemyAI);
		}
		ApplyPoolObjectDefaults(true, EnemyAI);
	}
}

void AAIManagerBase::ApplyPoolObjectDefaults(bool bApply, ABaseEnemyCharacter* PoolObject)
{
	if (!PoolObject) return;

	//apply defaults to put object into pool or retrieve it
	PoolObject->SetActorEnableCollision(bApply);
	PoolObject->SetActorHiddenInGame(bApply);
	PoolObject->SetActorTickEnabled(bApply);
	PoolObject->bIsInPool = bApply;
}



void AAIManagerBase::GetSpawnVolumes()
{
	UGameplayStatics::GetAllActorsOfClass(this, StaticVolumeSpawnerClass, SpawnerActors);// get actor of spawner and spawn volumes there
	int32 ArraySize = SpawnerActors.Num();
	if (ArraySize > 0)
	{
		int32 i = 0;
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		for (AActor* Object : SpawnerActors)
		{
			FTransform Transform = Object->GetActorTransform();
			AStaticAIVolume* Volume = GetWorld()->SpawnActor<AStaticAIVolume>(StaticVolumeClass,Transform,SpawnParams);
			Volume->Index = i;
			Volume->AIManagerRef = this;
			SpawnedVolumes.Push(Volume);
			i += 1;
		}
	}
	
}

void AAIManagerBase::OnNotifyVolumeBeginOverlap(int32 InIndex)
{
	AStaticAIVolume* CurrentVolume = SpawnedVolumes[InIndex];          // Storing Current Volume to get defaults
	AStaticAIVolume* PreviousVolume = SpawnedVolumes[LastUsedVolumeIndex];  // Getting Previous Volumes to remove it's spawned AI's

	CurrentVolumeIndex = InIndex;                                           // Updating Manager's variables to refer to for future iteration

	FVector VolumeLocation = CurrentVolume->GetActorLocation();        // Location to use for EQS Query

	int32 CurrVolNumAI = CurrentVolume->StaticAIPawns.Num();           // fetch current spawned AI for current volume to determine how many remain
	int32 PrevVolNumAI = PreviousVolume->StaticAIPawns.Num();          // fetch num of prev volume's spawned AI's to use them in current volume

	for (int32 i = 0; i < PrevVolNumAI; i++)                           // Iterate through num of prev volume's spawned AI's
	{
		StaticPool.Push(PreviousVolume->StaticAIPawns.Pop());          // Remove AI from Previous volume, Add to manager's array
	}

	if (CurrVolNumAI <= StaticPoolSize)                                // Check if AI num in current volume is already at max
	{
		int32 NumAIToSpawn = StaticPoolSize - CurrVolNumAI;            // Get number of AI's to spawn in current volume
		for (int32 i = 0; i < NumAIToSpawn; i++)                       // Iterate and spawn in remaining required AI Pawns
		{
			ABaseEnemyCharacter* AI = StaticPool.Pop();                // Add To current volume array
			
			UCustom_EnvQueryContext::QueryContextLocation = AI->GetActorLocation();

			RunEQS(CurrentVolume->GetActorLocation(), StaticEQS);     // Run EQS at root location of Current Volume and retrieve location 
			FVector SpawnLocation = FetchedLocation;              

			AI->SetActorLocation(SpawnLocation);                       // Set currently spawning AI's location at retrieved location

			ApplyPoolObjectDefaults(false, AI);                       // Make AI visible
			AI->Tags.Add("Static");                                   // To Identify AI's via category
			CurrentVolume->StaticAIPawns.Push(AI);
		}
		         
	}
}

void AAIManagerBase::OnNotifyVolumeEndOverlap(int32 OutIndex)
{
	LastUsedVolumeIndex = OutIndex;
}

void AAIManagerBase::RunEQS(FVector AtLocation, UEnvQuery* Query)
{
	if (!Query) return ;
	UEnvQueryManager* EQSManager = UEnvQueryManager::GetCurrent(this);
	if (!EQSManager) return ;

	FEnvQueryRequest QueryRequest(Query, this);

	EQSManager->RunQuery(QueryRequest, EEnvQueryRunMode::SingleResult, FQueryFinishedSignature::CreateUObject(this, &ThisClass::FetchEQSLocation));
}

void AAIManagerBase::FetchEQSLocation(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful())
	{
		FetchedLocation = Result->GetItemAsLocation(0);
		return;
	}
	DEBUG::PrintString("Couldn't find EQS Location",10.f,FColor::Red);
	FetchedLocation = FVector::ZeroVector;
	return;
	 
}


