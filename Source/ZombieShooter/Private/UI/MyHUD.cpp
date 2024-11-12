// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/CrosshairUI.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::BeginPlay()
{
	CrosshairWidget = CreateWidget<UCrosshairUI>(GetWorld(), CrosshairClass);
	if (CrosshairWidget)
	{
		CrosshairWidget->AddToViewport();
	}
	ReceiveBeginPlay();
}
