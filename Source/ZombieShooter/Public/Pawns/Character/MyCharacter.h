// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Pawns/Character/BaseCharacter.h"
#include "Logging/LogMacros.h"
#include "MyCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class AMyPlayerController;
class UDamageType;
struct FInputActionValue;
struct FHitResult;



DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AMyCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AMyCharacter();
#pragma region Overrides
protected:
	virtual void BeginPlay();
	virtual void Tick(float Delta) override;
	virtual void PossessedBy(AController* NewController) override;
public:
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

#pragma endregion

public:
#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Mesh", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

#pragma endregion

#pragma region Input

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* ADSAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* SwitchCameraAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Input")
	UInputAction* InteractAction;

#pragma endregion


#pragma region InputFunctions
protected:

	void HandleMoveInput(const FInputActionValue& Value);

	void HandleLookInput(const FInputActionValue& Value);

	void HandleJumpInput(const FInputActionValue& Value);

	UFUNCTION()
	void StartFire(const FInputActionValue& Value);
	void HandleFire();
	
	UFUNCTION()
	void EndFire(const FInputActionValue& Value);

	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void EndSprint(const FInputActionValue& Value);

	UFUNCTION()
	void StartADS(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void EndADS(const FInputActionValue& InputActionValue);

	void HandleSwitchCamera(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void StartCrouch(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void EndCrouch(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void StartInteract(const FInputActionValue& Value);
	UFUNCTION()
	void EndInteract(const FInputActionValue& Value);

#pragma endregion

#pragma region CoreVariables
public:
	
	//AMyPlayerController* MyController;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Player|Movement")
	float WalkSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Player|Movement")
	float SprintSpeed = 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Movement")
	float CrouchSpeed = 120.f;

	UPROPERTY(EditDefaultsOnly,  BlueprintReadOnly, Category = "Player|Trace")
	TArray <TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	float FireRate = 0.35f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	float TraceDistance = 200000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	float InteractionDistance = 200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	float TraceRadius = 12.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	bool bIsSingleFireWeapon = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|ADS")
	FVector LastADSLocation = FVector(0.f, 120.f, 0.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|ADS")
	float ADSFOV = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|ADS")
	float MaxPitch = 60.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|ADS")
	float MaxYaw = 45.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	float DamageAmount = 30.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	TSubclassOf<UDamageType> DamageClass;

	bool bADS;
	bool bCrouching;
protected:
	FTimerHandle FireTimerHandle;
	float LastFiredTime;
	float FirstFireDelay;
	TObjectPtr<AActor> InteractingActor;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	FVector RespawnLocation;
#pragma endregion

#pragma region CoreFunctions

	FHitResult DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug = false, 
		bool ForDuration  = false, float Duration = 2.f);
	FHitResult DoSphereTraceByObject(FVector Start, FVector End,float SphereRadius = 5.f, bool ShowDebug = false,
		bool ForDuration = false, float Duration = 2.f);
	void HandleInteraction(AActor* InActor);

	void SwitchCamera(bool bChangeStance=false);
	virtual void OnDeathEvent() override;
	void RespawnPlayer();
#pragma endregion

#pragma region Getters
public:
	USkeletalMeshComponent* GetWeaponMesh() const { return WeaponMesh; }
	UCameraComponent* GetCameraComponent() const { return Camera; }

#pragma endregion
};

