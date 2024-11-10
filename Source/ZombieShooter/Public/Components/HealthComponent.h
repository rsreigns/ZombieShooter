// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOMBIESHOOTER_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent();

#pragma region Overrides

#pragma endregion

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override;
#pragma region CoreVariables

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MaxHealth")
	float MaxHealth = 100.f;
	float CurrentHealth;
#pragma endregion

#pragma region CoreFunctions

public:	
	bool CastDamage(float DamageValue);

#pragma endregion

#pragma region Getters
public:

	FORCEINLINE float GetHealth() {return CurrentHealth;}

#pragma endregion

		
};
