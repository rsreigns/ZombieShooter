// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Character/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "DebugHelper.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComponent->MaxHealth = 100.f;

}


void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	OwningAnimInstance = GetMesh()->GetAnimInstance();
	if (OwningAnimInstance)
	{
		OwningAnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMontageEnd);
		OwningAnimInstance->OnMontageBlendingOut.AddDynamic(this, &ThisClass::OnMontageBlendOut);
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
	AController* EventInstigator, AActor* DamageCauser)
{
	bIsDead = HealthComponent->CastDamage(DamageAmount);
	if (bIsDead)
	{
		if (DeathMontage)
		{
		PlayMontage(DeathMontage);
		}
		return DamageAmount;
	}
	if (HitReactMontage.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, HitReactMontage.Num() - 1);
		UAnimMontage* HitMontage = HitReactMontage[Index];
		if (HitMontage)
		{
			OwningAnimInstance->StopAllMontages(0.f) ;
			PlayMontage(HitMontage);
			DEBUG::PrintString("Playing hit montage");
		}
	}

	return DamageAmount;
}


void ABaseCharacter::PlayMontage(UAnimMontage* MontageToPlay)
{
	if (MontageToPlay == DeathMontage)
	{
		OwningAnimInstance->StopAllMontages(0.f);
		PlayAnimMontage(MontageToPlay);
		return;
	}
	if (GetCharacterMovement()->IsFalling()) return;
	if (GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()) return;
	PlayAnimMontage(MontageToPlay);
}

void ABaseCharacter::OnMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{

}

void ABaseCharacter::OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == DeathMontage)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetAllBodiesBelowSimulatePhysics("root", true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		if(OwningAnimInstance)	OwningAnimInstance->StopAllMontages(0.4f);
		if (!GetController()->IsPlayerController())
		{
			GetController()->UnPossess();
			SetActorTickEnabled(false);
			if(HealthComponent) HealthComponent->SetActive(false);
		}
		DEBUG::PrintString("Montage End triggered");
		OnDeathEvent();
	}
}

void ABaseCharacter::OnDeathEvent()
{
}



