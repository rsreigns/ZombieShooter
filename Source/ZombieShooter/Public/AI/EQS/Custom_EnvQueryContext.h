// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "Custom_EnvQueryContext.generated.h"


UCLASS()
class ZOMBIESHOOTER_API UCustom_EnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
	static FVector QueryContextLocation;

};
