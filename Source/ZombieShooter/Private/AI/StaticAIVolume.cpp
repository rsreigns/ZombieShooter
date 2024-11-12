// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/StaticAIVolume.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

#include "Pawns/Character/MyCharacter.h"
#include "AI/AIManagerBase.h"


AStaticAIVolume::AStaticAIVolume()
{
	SphereOverlap = CreateDefaultSubobject<USphereComponent>(TEXT("SphereOverlapComponent"));
	SphereOverlap->SetSphereRadius(120.f);
	SphereOverlap->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereOverlap->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void AStaticAIVolume::BeginPlay()
{
	Super::BeginPlay();
	SphereOverlap->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
	SphereOverlap->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlap);
}


void AStaticAIVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaticAIVolume::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (MyCharacter) return;
	MyCharacter = Cast<AMyCharacter>(OtherActor);
	if (MyCharacter)
	{
		//AIManagerRef->OnNotifyVolumeBeginOverlap(Index);
	}
	return;
}

void AStaticAIVolume::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!MyCharacter) return;
	MyCharacter = nullptr;
	//AIManagerRef->OnNotifyVolumeEndOverlap(Index);
}
