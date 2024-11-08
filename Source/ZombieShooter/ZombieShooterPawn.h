// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ZombieShooterPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
struct FInputActionValue;
struct FHitResult;
struct FTimerHandle;
enum EObjectTypeQuery;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateVehicle, Log, All);


UCLASS()
class AZombieShooterPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;

#pragma endregion
protected:

#pragma region Input Actions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HandbrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAroundAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ResetVehicleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* FireAction;

#pragma endregion

public:
	AZombieShooterPawn();

#pragma region Overrides 

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;

#pragma endregion


protected:

#pragma region Input

	void Steering(const FInputActionValue& Value);

	void Throttle(const FInputActionValue& Value);

	void Brake(const FInputActionValue& Value);

	void StartBrake(const FInputActionValue& Value);
	void StopBrake(const FInputActionValue& Value);

	void StartHandbrake(const FInputActionValue& Value);
	void StopHandbrake(const FInputActionValue& Value);

	void LookAround(const FInputActionValue& Value);

	void ResetVehicle(const FInputActionValue& Value);

#pragma endregion

#pragma region Functions 

	UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle")
	void BrakeLights(bool bBraking);

	void StartShooting();
	void StopShooting();
	UFUNCTION()
	void FireEvent();

	FHitResult DoLineTraceByObject(FVector Start, FVector End,bool bShouldDrawTrace,bool ForDuration,float Duration = 2.f);

#pragma endregion

#pragma region CoreVariables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreVariables")
	bool bIsSingleFireWeapon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreVariables")
	float FireRate = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CoreVariables")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	float LastFiredTime;
	FTimerHandle FireTimerHandle;


#pragma endregion


public:
	FORCEINLINE USpringArmComponent* GetBackSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetChaosVehicleMovement() const { return ChaosVehicleMovement; }
};
