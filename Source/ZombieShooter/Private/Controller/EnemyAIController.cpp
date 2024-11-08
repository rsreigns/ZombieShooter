// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AEnemyAIController::AEnemyAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComp->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ThisClass::OnPerceptionUpdated);
}

void AEnemyAIController::OnPerceptionUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{

}
