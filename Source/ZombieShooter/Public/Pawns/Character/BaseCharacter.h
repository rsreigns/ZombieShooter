// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UAnimMontage;
class UAnimInstance;

UCLASS()
class ZOMBIESHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABaseCharacter();
#pragma region  Components
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	UHealthComponent* HealthComponent;

#pragma endregion

#pragma region Overrides

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion

#pragma region CoreVariables
	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;
	UPROPERTY(BlueprintReadOnly)
	UAnimInstance* OwningAnimInstance;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Animation|Montages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation|Montages")
	TArray<UAnimMontage*> HitReactMontage;
#pragma endregion

#pragma region CoreFunctions

	virtual void PlayMontage(UAnimMontage* MontageToPlay);
	UFUNCTION()
	void OnMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);
	virtual void OnDeathEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnReceiveDamage(float DamageRecieved);
#pragma endregion

#pragma region Getters
	FORCEINLINE UHealthComponent* GetHealthComp() { return HealthComponent; }
#pragma endregion

};
