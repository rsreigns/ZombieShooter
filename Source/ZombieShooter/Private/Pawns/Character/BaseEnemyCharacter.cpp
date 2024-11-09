// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Character/BaseEnemyCharacter.h"
#include "Controller/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
	GetCharacterMovement()->MaxWalkSpeed = 150.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;
}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//MyAIController = Cast<AEnemyAIController>(NewController);
	//if (MyAIController)
	//{
	//	MyAIController->RunBehaviorTree(BehaviorTree);
	//}
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


