// Copyright Epic Games, Inc. All Rights Reserved.

#include "Pawns/Vehicle/ZombieShooterPawn.h"
#include "Pawns/Vehicle/Wheels/ZombieShooterWheelFront.h"
#include "Pawns/Vehicle/Wheels/ZombieShooterWheelRear.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Pawns/Character/MyCharacter.h"
#include "Controller/MyPlayerController.h"
#include "GameFramework/DamageType.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "DebugHelper.h"

#define LOCTEXT_NAMESPACE "VehiclePawn"

DEFINE_LOG_CATEGORY(LogTemplateVehicle);

AZombieShooterPawn::AZombieShooterPawn()
{

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->SocketOffset.Z = 150.0f;
	SpringArm->bDoCollisionTest = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.0f;
	SpringArm->CameraLagMaxDistance = 50.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Vehicle"));

	ChaosVehicleMovement = CastChecked<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->MaxHealth = 500.f;

	GetMesh()->OnComponentHit.AddDynamic(this, &ThisClass::ComponentHit);

	WeaponComp = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
	WeaponComp->bIsSingleFireWeapon = false;
	WeaponComp->FireRate = 0.1f;
	WeaponComp->DamageAmount = 50.f;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComp->SetupAttachment(GetMesh(), "WeaponSocket");
}
void AZombieShooterPawn::Tick(float Delta)
{
	Super::Tick(Delta);
	bool bMovingOnGround = ChaosVehicleMovement->IsMovingOnGround();
	GetMesh()->SetAngularDamping(bMovingOnGround ? 0.0f : 3.0f);
}

void AZombieShooterPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetActorTickEnabled(true);
	//if (!MyController) MyController = Cast<AMyPlayerController>(NewController);

}

void AZombieShooterPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AMyPlayerController* MyController = Cast<AMyPlayerController>(GetController());
	if (!Subsystem) Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyController->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(VehicleMappingContext, 0);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// steering 
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &AZombieShooterPawn::Steering);
		EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &AZombieShooterPawn::Steering);

		// throttle 
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &AZombieShooterPawn::Throttle);
		EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &AZombieShooterPawn::Throttle);

		// break 
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &AZombieShooterPawn::Brake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Started, this, &AZombieShooterPawn::StartBrake);
		EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &AZombieShooterPawn::StopBrake);

		// handbrake 
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Started, this, &AZombieShooterPawn::StartHandbrake);
		EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &AZombieShooterPawn::StopHandbrake);

		// look around 
		EnhancedInputComponent->BindAction(LookAroundAction, ETriggerEvent::Triggered, this, &AZombieShooterPawn::LookAround);


		// reset the vehicle 
		EnhancedInputComponent->BindAction(ResetVehicleAction, ETriggerEvent::Triggered, this, &AZombieShooterPawn::ResetVehicle);

		//Fire 
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComp, &UWeaponComponent::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComp, &UWeaponComponent::StopFire);

		EnhancedInputComponent->BindAction(ExitAction, ETriggerEvent::Started, this, &ThisClass::ExitVehicle);

	}
}


#pragma region DefaultInputFunctions
void AZombieShooterPawn::Steering(const FInputActionValue& Value)
{
	float SteeringValue = Value.Get<float>();
	ChaosVehicleMovement->SetSteeringInput(SteeringValue);
}

void AZombieShooterPawn::Throttle(const FInputActionValue& Value)
{
	float ThrottleValue = Value.Get<float>();
	ChaosVehicleMovement->SetThrottleInput(ThrottleValue);
}

void AZombieShooterPawn::Brake(const FInputActionValue& Value)
{
	float BreakValue = Value.Get<float>();
	ChaosVehicleMovement->SetBrakeInput(BreakValue);
}

void AZombieShooterPawn::StartBrake(const FInputActionValue& Value)
{
	BrakeLights(true);
}

void AZombieShooterPawn::StopBrake(const FInputActionValue& Value)
{
	BrakeLights(false);
	ChaosVehicleMovement->SetBrakeInput(0.0f);
}

void AZombieShooterPawn::StartHandbrake(const FInputActionValue& Value)
{
	ChaosVehicleMovement->SetHandbrakeInput(true);
	BrakeLights(true);
}

void AZombieShooterPawn::StopHandbrake(const FInputActionValue& Value)
{
	ChaosVehicleMovement->SetHandbrakeInput(false);
	BrakeLights(false);
}

void AZombieShooterPawn::LookAround(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void AZombieShooterPawn::ResetVehicle(const FInputActionValue& Value)
{
	FVector ResetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);

	FRotator ResetRotation = GetActorRotation();
	ResetRotation.Pitch = 0.0f;
	ResetRotation.Roll = 0.0f;
	
	SetActorTransform(FTransform(ResetRotation, ResetLocation, FVector::OneVector), false, nullptr, ETeleportType::TeleportPhysics);

	GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
}

#pragma endregion

void AZombieShooterPawn::ExitVehicle(const FInputActionValue& Value)
{
	AActor* ActorToFind = UGameplayStatics::GetActorOfClass(GetWorld(),AMyCharacter::StaticClass());
	MyCharacter = Cast<AMyCharacter>(ActorToFind);
	if (MyCharacter)
	{
		FVector OutLocation = GetMesh()->GetSocketLocation("Out");
		MyCharacter->SetActorLocation(OutLocation);
		MyCharacter->SetActorHiddenInGame(false);
		SetActorTickEnabled(false);
		GetController()->Possess(MyCharacter);
	}
}


void AZombieShooterPawn::StartShooting(const FInputActionValue& Value)
{
	WeaponComp->StartFire();
}

void AZombieShooterPawn::StopShooting(const FInputActionValue& Value)
{
	WeaponComp->StopFire();
}

void AZombieShooterPawn::ComponentHit( UPrimitiveComponent* HitComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	float CurrentVelocity = GetVelocity().Size2D();
	if (CurrentVelocity < 150.f) return;
	if (HitPawn && HitPawn == Cast<APawn>(OtherActor)) return;
	HitPawn = Cast<APawn>(OtherActor);
	float HitSpeedMulti = CurrentVelocity/ ChaosVehicleMovement->GetMaxSpeed();
	if (HitPawn && !HitPawn->IsPlayerControlled())
	{
		UCapsuleComponent* PawnCapsule = HitPawn->GetComponentByClass<UCapsuleComponent>();
		PawnCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		PawnCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
		float BaseDamage = 10.f;
		float NewDamage = BaseDamage * CarHitDamageMultiplier * HitSpeedMulti;
		UGameplayStatics::ApplyDamage(OtherActor, NewDamage, GetInstigator()->GetController(),this, DamageClass);
		return;
	}
}

FHitResult AZombieShooterPawn::DoLineTraceByObject(FVector Start, FVector End, bool bShouldDrawTrace, bool ForDuration, float Duration)
{
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	if (bShouldDrawTrace)
	{
		if (ForDuration)
		{
			DebugType = EDrawDebugTrace::ForDuration;
		}
		else DebugType = EDrawDebugTrace::ForOneFrame;
	}
	FHitResult OutHit;

		UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, 
				false, TArray<AActor*>(),DebugType,OutHit,true,FColor::Red,FColor::Green,Duration );

	return OutHit;
}

#undef LOCTEXT_NAMESPACE