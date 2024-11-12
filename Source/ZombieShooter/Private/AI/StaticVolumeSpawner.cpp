// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StaticVolumeSpawner.h"
#include "Components/CapsuleComponent.h"
#include "Pawns/Character/MyCharacter.h"

AStaticVolumeSpawner::AStaticVolumeSpawner()
{
	//PrimaryActorTick.bCanEverTick = true;
	DebugCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	

}


void AStaticVolumeSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	
}


//void AStaticVolumeSpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

