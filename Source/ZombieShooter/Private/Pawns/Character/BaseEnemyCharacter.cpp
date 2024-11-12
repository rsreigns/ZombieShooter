// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Character/BaseEnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "AI/AIManagerBase.h"

#include "DebugHelper.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;

}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	float NewSpeed = FMath::RandRange(BaseWalkSpeed - 50, BaseWalkSpeed + 50);
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemyCharacter::OnDeathEvent()
{
	FTimerHandle DeathTimerHandle;
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ThisClass::SendToPool, 10.f);
}

void ABaseEnemyCharacter::SendToPool()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	Destroy();
}


