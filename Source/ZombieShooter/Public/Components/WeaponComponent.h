// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

struct FTimerHandle;
struct FHitResult;
class UCameraComponent;
class UParticleSystem;


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


	FTimerHandle FireTimerHandle;
	float LastFiredTime;
	float FirstFireDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	TArray <TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Trace")
	bool bIsSingleFireWeapon = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	float DamageAmount = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Damage")
	TSubclassOf<UDamageType> DamageClass;

	FHitResult DoLineTraceByObject(FVector Start, FVector End, bool ShowDebug = false,
		bool ForDuration = false, float Duration = 2.f);

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

	void StartFire();
	UFUNCTION(BlueprintCallable)
	void HandleFire();
	void StopFire();

	TObjectPtr<UCameraComponent> CameraRef;
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
		
};
