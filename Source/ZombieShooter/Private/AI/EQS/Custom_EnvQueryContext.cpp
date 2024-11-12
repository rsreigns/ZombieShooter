// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EQS/Custom_EnvQueryContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Direction.h"

FVector UCustom_EnvQueryContext::QueryContextLocation = FVector::ZeroVector;

void UCustom_EnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	UEnvQueryItemType_Direction::SetContextHelper(ContextData, QueryContextLocation);
}
