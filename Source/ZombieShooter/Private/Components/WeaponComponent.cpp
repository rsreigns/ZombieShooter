// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

#include "Pawns/Character/MyCharacter.h"
#include "Pawns/Vehicle/ZombieShooterPawn.h"
#include "Pawns/Character/BaseEnemyCharacter.h"

#include "DebugHelper.h"
#include "DrawDebugHelpers.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		ActorsToIgnore.Add(GetOwner());
		AMyCharacter* MyChar = Cast<AMyCharacter>(GetOwner());
		if (MyChar)
		{
			bIsCar = false;
			CameraRef = MyChar->GetCameraComponent();
			WeaponMesh = MyChar->GetWeaponMesh();
		}
		else
		{
			AZombieShooterPawn* CarPawn = Cast<AZombieShooterPawn>(GetOwner());
			{
				if (CarPawn)
				{
					bIsCar = true;
					CameraRef = CarPawn->GetCamera();
					WeaponMesh = CarPawn->GetWeaponMesh();
				}
			}
		}
	}
	
	AActor* PlayerChar = UGameplayStatics::GetActorOfClass(GetWorld(),MyCharClass);
	if(PlayerChar) ActorsToIgnore.Add(PlayerChar);
	
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UWeaponComponent::StartFire()
{
	
	if (GetWorld())
	{
		FirstFireDelay = FMath::Max(LastFiredTime + FireRate - GetWorld()->TimeSeconds, 0.f);
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::HandleFire, FireRate, !bIsSingleFireWeapon, FirstFireDelay);
	}
}

void UWeaponComponent::HandleFire()
{
	if (bIsCar)
	{
		HandleAutoFire();
		return;
	}
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	FVector StartPoint = CameraRef->GetComponentLocation();
	FVector EndPoint = StartPoint + CameraRef->GetForwardVector() * 2000000.f;

	LastFiredTime = GetWorld()->TimeSeconds;

	FHitResult OutHit = DoLineTraceByObject(StartPoint, EndPoint);

	FVector HitLoc = OutHit.Location;
	FHitResult RealHit = DoLineTraceByObject(MuzzleLocation, HitLoc);
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn)
	{
		MyPawn->MakeNoise(1.f, MyPawn, GetOwner()->GetActorLocation());
		CastDamageToHitActor(OutHit);
	}
	SpawnEffects(OutHit);
	
}

void UWeaponComponent::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	FireTimerHandle.Invalidate();
}

void UWeaponComponent::HandleAutoFire()
{
	if (!CameraRef || !WeaponMesh) { return; }
	DrawDebugBox(GetWorld(), GetOwner()->GetActorLocation(), BoxExtent, FColor::Blue, false, 1.f);
	TArray<AActor*> OutActors;
	bool bFoundOverlap = UKismetSystemLibrary::BoxOverlapActors(this, GetOwner()->GetActorLocation(),
		BoxExtent, TraceObjectTypes, ABaseEnemyCharacter::StaticClass(), ActorsToIgnore, OutActors);
	AActor* NearestActor = FindNearestEnemy(OutActors);
	if (NearestActor)
	{
		FVector FoundActorLocation = NearestActor->GetActorLocation();
		FVector MuzzleLoc = WeaponMesh->GetSocketLocation("Muzzle");
		FVector Direction = FoundActorLocation - MuzzleLoc;

		FVector EndPoint = MuzzleLoc + Direction * 2000000.f;

		FHitResult OutHit = DoSphereTraceByObject(10.f, MuzzleLoc, EndPoint);
		if (OutHit.bBlockingHit)
		{
			APawn* HitPawn = Cast<APawn>(OutHit.GetActor());
			if (HitPawn)
			{
				HitPawn->MakeNoise(1.f, HitPawn, GetOwner()->GetActorLocation());
				CastDamageToHitActor(OutHit);
			}
			SpawnEffects(OutHit);
		}
	}


}

AActor* UWeaponComponent::FindNearestEnemy(TArray<AActor*> OverlappedActors)
{
	AActor* NearestActor = nullptr;
	float MinDistance = FLT_MAX;

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	for (AActor* Actor : OverlappedActors)
	{
		if (Actor)
		{
			float Distance = FVector::Dist(OwnerLocation, Actor->GetActorLocation());
			if (Distance < MinDistance)
			{
				ABaseEnemyCharacter* EnemyChar = Cast<ABaseEnemyCharacter>(Actor);
				if (EnemyChar)
				{
					if (EnemyChar->bIsDead) continue;
				}
				MinDistance = Distance;
				NearestActor = Actor;
			}
		}
	}

	return NearestActor;
}

void UWeaponComponent::CastDamageToHitActor(FHitResult InHit)
{
	if (InHit.bBlockingHit)
	{
		UGameplayStatics::ApplyDamage(InHit.GetActor(), DamageAmount, GetOwner()->GetInstigatorController(), GetOwner(), DamageClass);

		FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
		FVector HitLoc = InHit.Location;

		FVector NormalDirection = HitLoc - MuzzleLocation;

		if (InHit.GetComponent() && InHit.GetComponent()->IsSimulatingPhysics() && InHit.GetComponent()->IsPhysicsCollisionEnabled())
		{
			InHit.GetComponent()->AddForceAtLocation(NormalDirection * 500.f, HitLoc);
		}
	}
}

void UWeaponComponent::SpawnEffects(FHitResult InHit)
{
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	FVector HitLocation = InHit.Location;
	if (BeamEffect)
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamEffect, MuzzleLocation);
		if (Beam)
		{
			FVector EndPoint = InHit.bBlockingHit ? InHit.Location : InHit.TraceEnd;
			Beam->SetVectorParameter("BeamEnd", EndPoint);
		}
	}
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, "Muzzle", MuzzleLocation, InHit.ImpactNormal.Rotation(), EAttachLocation::SnapToTarget);
	}
	if (Cast<APawn>(InHit.GetActor()))
	{
		if (BloodEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, HitLocation);
		}
	}
	else
	{
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitLocation);
		}
	}
	if (FireSound)
	{
		UGameplayStatics::SpawnSound2D(this, FireSound);
	}

}

FHitResult UWeaponComponent::DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug, bool ForDuration, float Duration)
{
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	if (ShowDebug)
	{
		DebugType = EDrawDebugTrace::ForOneFrame;
		if (ForDuration)
		{
			DebugType = EDrawDebugTrace::ForDuration;
		}
	}
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects
	(this, Start, End, TraceObjectTypes, false, TArray<AActor*>(), DebugType, OutHit, true, FColor::Red, FColor::Green, Duration);

	return OutHit;
}

FHitResult UWeaponComponent::DoSphereTraceByObject(float SphereRadius, FVector Start, FVector End, bool ShowDebug, bool ForDuration, float Duration)
{
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	if (ShowDebug)
	{
		DebugType = EDrawDebugTrace::ForOneFrame;
		if (ForDuration)
		{
			DebugType = EDrawDebugTrace::ForDuration;
		}
	}
	FHitResult OutHit;
	UKismetSystemLibrary::SphereTraceSingleForObjects
	(this, Start, End,SphereRadius, TraceObjectTypes, false, TArray<AActor*>(), DebugType, OutHit, true, FColor::Red, FColor::Green, Duration);

	return OutHit;
}
