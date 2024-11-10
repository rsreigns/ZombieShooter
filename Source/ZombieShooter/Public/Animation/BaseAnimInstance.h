// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"
class ABaseCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class ZOMBIESHOOTER_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	ABaseCharacter* OwningChar;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	UCharacterMovementComponent* OwningCharMovementComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	float GroundSpeed;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	float Direction;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	float ZVelocity;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Player")
	bool bIsFalling;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
};
