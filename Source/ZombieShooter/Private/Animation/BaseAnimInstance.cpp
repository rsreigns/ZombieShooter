// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BaseAnimInstance.h"
#include "Pawns/Character/BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningChar = Cast<ABaseCharacter>(TryGetPawnOwner());
	if(OwningChar) OwningCharMovementComponent = OwningChar->GetCharacterMovement();
}

void UBaseAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwningChar)
	{
		GroundSpeed = OwningChar->GetVelocity().Size2D();
		ZVelocity = OwningChar->GetVelocity().Z;
		Direction = UKismetAnimationLibrary::CalculateDirection(OwningChar->GetVelocity(), OwningChar->GetActorRotation());
	}
	if (OwningCharMovementComponent)
	{
		bIsFalling = OwningCharMovementComponent->IsFalling();
	}
}
