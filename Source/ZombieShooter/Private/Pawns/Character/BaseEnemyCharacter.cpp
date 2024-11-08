// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Character/BaseEnemyCharacter.h"


ABaseEnemyCharacter::ABaseEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

