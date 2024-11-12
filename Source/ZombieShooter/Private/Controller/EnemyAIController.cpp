// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"

#include "DebugHelper.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComp->ConfigureSense(*SightConfig);
	PerceptionComp->ConfigureSense(*HearConfig);

	SetGenericTeamId(FGenericTeamId(1));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();


	PerceptionComp->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	if (CrowdComp)
	{
		CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::Enabled);
		switch (AvoidanceQuality)
		{
		case 1: 	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2:	    CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3:	    CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: 	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default: 	CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		}
		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* OtherPawn = Cast<APawn>(&Other);
	const IGenericTeamAgentInterface* OtherInterface = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());
	if (OtherInterface && OtherInterface->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}
	return ETeamAttitude::Friendly;
}

void AEnemyAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	if (BBComp)
	{
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (Actor && Stimulus.WasSuccessfullySensed())
			{
				BBComp->SetValueAsObject("Player", Actor);
			}
			else
			{
				BBComp->ClearValue("Player");
				BBComp->SetValueAsVector("LastKnownLocation", Stimulus.StimulusLocation);
			}
		}
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				BBComp->SetValueAsVector("LastKnownLocation", Stimulus.StimulusLocation);
			}
			else
			{
				BBComp->ClearValue("LastKnownLocation");
			}
		}
	}
	//if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	//{
	//	if (Actor && Stimulus.WasSuccessfullySensed())
	//	{
	//		FAIMoveRequest MoveRequest;
	//		MoveRequest.SetGoalActor(Actor);
	//		MoveRequest.SetUsePathfinding(true);
	//		MoveRequest.SetAcceptanceRadius(10.f);
	//		GetPawn()->GetActorLocation();
	//		MoveTo(MoveRequest);
	//	}
	//	else
	//	{
	//		
	//	}
	//}
	//else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
	//{
	//	if (Stimulus.WasSuccessfullySensed())
	//	{
	//		FAIMoveRequest MoveRequest;
	//		MoveRequest.SetGoalLocation(Stimulus.StimulusLocation);
	//		MoveRequest.SetUsePathfinding(true);
	//		MoveRequest.SetAcceptanceRadius(10.f);
	//		MoveTo(MoveRequest);
	//	}
	//	else
	//	{
	//		
	//	}
	//}


}



