// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pawns/Character/MyCharacter.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "Pawns/Vehicle/ZombieShooterPawn.h"
#include "Controller/MyPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"

#include "DebugHelper.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


AMyCharacter::AMyCharacter()
{

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponSocket");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon Component"));
	WeaponComp->bIsSingleFireWeapon = true;
	WeaponComp->FireRate = 0.35f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitialTransform = GetActorTransform();
}

void AMyCharacter::Tick(float Delta)
{
	Super::Tick(Delta);
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetActorTickEnabled(true);
	//if (!MyController) MyController = Cast<AMyPlayerController>(NewController);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (MyController)
	{
		if (!Subsystem) Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer());
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveInput);
			InputComp->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput);
			InputComp->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::HandleJumpInput);

			InputComp->BindAction(SprintAction, ETriggerEvent::Started, this, &ThisClass::StartSprint);
			InputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::EndSprint);

			InputComp->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::StartCrouch);
			InputComp->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ThisClass::EndCrouch);

			InputComp->BindAction(FireAction, ETriggerEvent::Started, this, &ThisClass::StartFire);
			InputComp->BindAction(FireAction, ETriggerEvent::Completed, this, &ThisClass::EndFire);

			InputComp->BindAction(ADSAction, ETriggerEvent::Started, this, &ThisClass::StartADS);
			InputComp->BindAction(ADSAction, ETriggerEvent::Completed, this, &ThisClass::EndADS);

			InputComp->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::StartInteract);
			InputComp->BindAction(InteractAction, ETriggerEvent::Completed, this, &ThisClass::EndInteract);

			InputComp->BindAction(SwitchCameraAction, ETriggerEvent::Completed, this, &ThisClass::HandleSwitchCamera);
		}
	}

}


void AMyCharacter::HandleMoveInput(const FInputActionValue& Value)
{
	if (bADS) return;
	FVector2D MovementVector = Value.Get<FVector2D>();
	FRotator MovementDirection = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	if (MovementVector.Y != 0)
	{
		FVector ForwardVector = MovementDirection.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardVector, MovementVector.Y);
	}
	if (MovementVector.X != 0)
	{
		FVector RightVector = MovementDirection.RotateVector(FVector::RightVector);
		AddMovementInput(RightVector, MovementVector.X);
	}
}

void AMyCharacter::HandleLookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AMyCharacter::HandleJumpInput(const FInputActionValue& Value)
{
	Jump();
}

void AMyCharacter::StartFire(const FInputActionValue& Value)
{
	if (bADS)
	{
		WeaponComp->StartFire();
	}
}

void AMyCharacter::EndFire(const FInputActionValue& Value)
{
	WeaponComp->StopFire();
}
void AMyCharacter::StartSprint(const FInputActionValue& Value)
{
	if (!bADS)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}
void AMyCharacter::EndSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
void AMyCharacter::StartADS(const FInputActionValue& InputActionValue)
{
	float CurrentWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (CurrentWalkSpeed <= WalkSpeed)
	{
		SetActorRotation(Camera->GetComponentQuat(), ETeleportType::TeleportPhysics);
		bADS = true;
		SwitchCamera();
	}
}
void AMyCharacter::EndADS(const FInputActionValue& InputActionValue)
{
	float CurrentWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (CurrentWalkSpeed <= WalkSpeed)
	{
		bADS = false;
		SwitchCamera();
		WeaponComp->StopFire();
	}
}
void AMyCharacter::HandleSwitchCamera(const FInputActionValue& InputActionValue)
{
	SwitchCamera(true);
}
void AMyCharacter::SwitchCamera(bool ChangeStance)
{
	FVector NewOffset = LastADSLocation;
	if (bADS)
	{
		if (ChangeStance) NewOffset *= -1;
		LastADSLocation = NewOffset;
		Camera->FieldOfView = ADSFOV;
	}
	else
	{
		NewOffset = FVector(0.f,0.f,65.f);
		Camera->FieldOfView = 90.f;
	}
	SpringArm->SocketOffset = NewOffset;
}
void AMyCharacter::OnDeathEvent()
{
	RespawnPlayer();
}
void AMyCharacter::RespawnPlayer()
{
	BP_OnDeath();
}

void AMyCharacter::StartCrouch(const FInputActionValue& InputActionValue)
{
	bCrouching = true;
	GetCharacterMovement()->MaxWalkSpeed = CrouchSpeed;
}
void AMyCharacter::EndCrouch(const FInputActionValue& InputActionValue)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bCrouching = false;
}

void AMyCharacter::StartInteract(const FInputActionValue& Value)
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * InteractionDistance;
	FHitResult Hit = DoSphereTraceByObject(Start,End,TraceRadius);
	InteractingActor = Hit.GetActor();
	HandleInteraction(Hit.GetActor());
}
void AMyCharacter::HandleInteraction(AActor* InActor)
{
	if (InActor)
	{
		AZombieShooterPawn* VehiclePawn = Cast<AZombieShooterPawn>(InActor);
		if (VehiclePawn)
		{
			SetActorHiddenInGame(true);
			SetActorTickEnabled(false);
			GetController()->Possess(VehiclePawn);
		}
	}
}

void AMyCharacter::EndInteract(const FInputActionValue& Value)
{
	InteractingActor = nullptr;
}

#pragma region TraceLogic
FHitResult AMyCharacter::DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug, bool ForDuration, float Duration)
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

FHitResult AMyCharacter::DoSphereTraceByObject(FVector Start, FVector End, float SphereRadius, bool ShowDebug, bool ForDuration, float Duration)
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
	UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, TraceRadius, TraceObjectTypes,
		false,TArray<AActor*>() ,DebugType,OutHit ,true, FColor::Red, FColor::Green ,Duration );

	return OutHit;
}

#pragma endregion