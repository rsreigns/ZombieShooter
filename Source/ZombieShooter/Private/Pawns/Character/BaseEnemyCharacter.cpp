// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Character/BaseEnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"


#include "DebugHelper.h"

ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;

}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


