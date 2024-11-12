// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

struct FTimerHandle;
struct FHitResult;
class UCameraComponent;
class UParticleSystem;
class AMyCharacter;
class ABaseEnemyCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, Blueprintable) )
class ZOMBIESHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UWeaponComponent();

protected:
	
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

public:	

#pragma region Global Variables
	FTimerHandle FireTimerHandle;
	float LastFiredTime;
	float FirstFireDelay;
	TObjectPtr<UCameraComponent> CameraRef;
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
	bool bIsCar = false;
	TArray<AActor*> ActorsToIgnore;

#pragma endregion

#pragma region CoreVariables

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	TArray <TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	bool bIsSingleFireWeapon = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	float DamageAmount = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	TSubclassOf<UDamageType> DamageClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	float FireRate = 0.12f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Effects")
	UParticleSystem* MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Effects")
	UParticleSystem* BloodEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Effects")
	UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Effects")
	UParticleSystem* BeamEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Effects|Sound")
	USoundBase* FireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Car")
	FVector BoxExtent = FVector(1000.f, 1000.f, 1000.f);
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Car")
	TSubclassOf<AMyCharacter> MyCharClass;



#pragma endregion

#pragma region CoreFunction

	void StartFire();
	UFUNCTION(BlueprintCallable)
	void HandleFire();
	void StopFire();

	void HandleAutoFire();
	AActor* FindNearestEnemy(TArray<AActor*> OverlappedActors);

	void CastDamageToHitActor(FHitResult InHit);

	void SpawnEffects(FHitResult InHit);

	FHitResult DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug = false,
		bool ForDuration = false, float Duration = 2.f);
	FHitResult DoSphereTraceByObject(float SphereRadius, FVector Start, FVector End, bool ShowDebug = false,
		bool ForDuration = false, float Duration = 2.f);
	

#pragma endregion

		
};
