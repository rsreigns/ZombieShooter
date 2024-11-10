// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/BaseAnimInstance.h"
#include "MyAnimInstance.generated.h"
class AMyCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UMyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere,Category="Player")
	AMyCharacter* OwningPlayerChar;
	FRotator Rotation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	float Yaw;
	UPROPERTY(EditDefaultsOnly,  Category = "Player")
	float YawRange= 120.f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	float Pitch;
	UPROPERTY(EditDefaultsOnly,  Category = "Player")
	float PitchRange = 60.f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	bool bIsADS;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	bool bIsCrouching;


public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
};
