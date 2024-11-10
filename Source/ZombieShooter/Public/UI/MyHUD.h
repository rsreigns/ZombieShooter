// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UCrosshairUI;
class UUserWidget;

UCLASS()
class ZOMBIESHOOTER_API AMyHUD : public AHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetArray;
public:
	AMyHUD();
	virtual void BeginPlay() override;

	UCrosshairUI* CrosshairWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Classes")
	TSubclassOf<UCrosshairUI> CrosshairClass;
};
