// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "ZombieShooterPawn.generated.h"

class AMyPlayerController;
class AMyCharacter;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UChaosWheeledVehicleMovementComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UDamageType;
class UHealthComponent;
class UWeaponComponent;

struct FInputActionValue;
struct FHitResult;
struct FTimerHandle;
enum EObjectTypeQuery;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateVehicle, Log, All);


UCLASS()
class AZombieShooterPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

#pragma region Overrides 

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

#pragma endregion

#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	TObjectPtr<UChaosWheeledVehicleMovementComponent> ChaosVehicleMovement;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Components")
	UHealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Components")
	UWeaponComponent* WeaponComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Components")
	USkeletalMeshComponent* WeaponMeshComp;


#pragma endregion
protected:

#pragma region Input Actions

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputMappingContext* VehicleMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* HandbrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* LookAroundAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* ResetVehicleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* ExitAction;

#pragma endregion

public:
	AZombieShooterPawn();


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

	void ExitVehicle(const FInputActionValue& Value);

#pragma endregion

#pragma region Functions 

	UFUNCTION(BlueprintImplementableEvent, Category = "Vehicle")
	void BrakeLights(bool bBraking);

	void StartShooting();
	void StopShooting();

	UFUNCTION()
	void ComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FHitResult DoLineTraceByObject(FVector Start, FVector End,bool bShouldDrawTrace,bool ForDuration,float Duration = 2.f);

#pragma endregion

#pragma region CoreVariables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Weapon")
	bool bIsSingleFireWeapon = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Weapon")
	float FireRate = 0.25f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Weapon")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Car")
	float CarHitDamageMultiplier = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Car")
	TSubclassOf<UDamageType> DamageClass;
	TObjectPtr<APawn> HitPawn;
	TObjectPtr<AMyCharacter> MyCharacter;
	//TObjectPtr<AMyPlayerController> MyController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	float LastFiredTime;
	FTimerHandle FireTimerHandle;
public:
		bool bIsDestroyed = false;

#pragma endregion


public:
	FORCEINLINE USpringArmComponent* GetBackSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE const TObjectPtr<UChaosWheeledVehicleMovementComponent>& GetChaosVehicleMovement() const { return ChaosVehicleMovement; }
	FORCEINLINE UHealthComponent* GetHealthComponent() { return HealthComponent; }
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() { return WeaponMeshComp; }
};
