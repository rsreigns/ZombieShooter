

#include "Components/HealthComponent.h"

#include "DebugHelper.h"
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentHealth = MaxHealth;
}



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner())
	{
	DEBUG::PrintString(FString::Printf(TEXT("%s 's ,Current Health = %f "),*GetOwner()->GetName(), CurrentHealth),0.f);
	}


}

bool UHealthComponent::CastDamage(float DamageValue)
{
	if (CurrentHealth < 0)
	{
		Deactivate();
		return true;
	}
	CurrentHealth = CurrentHealth - DamageValue;
	DEBUG::PrintString(FString::Printf(TEXT("Damage Recieved : %f "), DamageValue), 7.f,FColor::Red);
	if (CurrentHealth <= 0)
	{
		return true;
	}
	return false;
}

