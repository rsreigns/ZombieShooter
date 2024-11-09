// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	TeamID = FGenericTeamId(0);
}

FGenericTeamId AMyPlayerController::GetGenericTeamId() const
{
	return TeamID;
}
