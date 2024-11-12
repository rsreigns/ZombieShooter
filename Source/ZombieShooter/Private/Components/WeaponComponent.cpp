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
		AMyCharacter* MyChar = Cast<AMyCharacter>(GetOwner());
		if (MyChar)
		{
			CameraRef = MyChar->GetCameraComponent();
			WeaponMesh = MyChar->GetWeaponMesh();
		}
		else
		{
			AZombieShooterPawn* CarPawn = Cast<AZombieShooterPawn>(GetOwner());
			{
				if (CarPawn)
				{
					
					CameraRef = CarPawn->GetCamera();
					WeaponMesh = CarPawn->GetWeaponMesh();
				}
			}
		}
	}
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UWeaponComponent::StartFire()
{
	FirstFireDelay = FMath::Max(LastFiredTime + FireRate - GetWorld()->TimeSeconds, 0.f);
	if (GetWorld())
	{
		DEBUG::PrintString(FString::Printf(TEXT("Delay : %f"), FirstFireDelay),5.f,FColor::Black);
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ThisClass::HandleFire, FireRate, !bIsSingleFireWeapon, FirstFireDelay);
	}
}

void UWeaponComponent::HandleFire()
{
	FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	FVector StartPoint = CameraRef->GetComponentLocation();
	FVector EndPoint = StartPoint + CameraRef->GetForwardVector() * 2000000.f;

	LastFiredTime = GetWorld()->TimeSeconds;

	FHitResult OutHit = DoLineTraceByObject(StartPoint, EndPoint);
#pragma region Effects
	if (BeamEffect)
	{
		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamEffect,MuzzleLocation);
		if (Beam)
		{
			Beam->SetVectorParameter("BeamEnd", EndPoint);
		}
	}
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, WeaponMesh, "Muzzle", MuzzleLocation, OutHit.ImpactNormal.Rotation(), EAttachLocation::SnapToTarget);
	}
	if (Cast<APawn>(OutHit.GetActor()))
	{
		if (BloodEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodEffect, OutHit.Location);
		}
	}
	else
	{
		if (ImpactEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, OutHit.Location);
		}
	}
	if (FireSound)
	{
		UGameplayStatics::SpawnSound2D(this, FireSound);
	}

#pragma endregion
	FVector HitLoc = OutHit.Location;
	FHitResult RealHit = DoLineTraceByObject(MuzzleLocation, HitLoc);
	APawn* MyPawn = Cast<APawn>(GetOwner());
	if(MyPawn)  MyPawn->MakeNoise(1.f, MyPawn, GetOwner()->GetActorLocation());
	
	if (OutHit.bBlockingHit)
	{
		UGameplayStatics::ApplyDamage(OutHit.GetActor(), DamageAmount, GetOwner()->GetInstigatorController(),GetOwner(), DamageClass);
		FVector NormalDirection = HitLoc - MuzzleLocation;
		if (OutHit.GetComponent() && OutHit.GetComponent()->IsSimulatingPhysics() && OutHit.GetComponent()->IsPhysicsCollisionEnabled())
		{
			OutHit.GetComponent()->AddForceAtLocation(NormalDirection * 500.f, OutHit.Location);
		}
	}		
	
}

void UWeaponComponent::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	FireTimerHandle.Invalidate();
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
