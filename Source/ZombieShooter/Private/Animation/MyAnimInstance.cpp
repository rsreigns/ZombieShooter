// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MyAnimInstance.h"
#include "Pawns/Character/MyCharacter.h"
#include "KismetAnimationLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwningPlayerChar = Cast<AMyCharacter>(OwningChar);
}

void UMyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (OwningPlayerChar)
	{
		Rotation = OwningPlayerChar->GetBaseAimRotation() - OwningPlayerChar->GetActorRotation();
		Yaw = FMath::Clamp(FMath::UnwindDegrees(Rotation.Yaw), -YawRange, YawRange);
		Pitch = FMath::Clamp(FMath::UnwindDegrees(Rotation.Pitch), -PitchRange, PitchRange);
		bIsADS = OwningPlayerChar->bADS;
		bIsCrouching = OwningPlayerChar->bCrouching;
	}
}
