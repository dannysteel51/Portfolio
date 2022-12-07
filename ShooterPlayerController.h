// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AShooterPlayerController();

protected:

	virtual void BeginPlay() override;	

private: 
	/** Reference to the overall HUD overlay blueprint class*/
	// This is Blueprint variable we create in c++ and set in the editor
	// Once it is set, it get checked in the .cpp file, allowing the
	// createwidget to be called
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> HUDOverlayClass;	

	/** Variable to hold the HUD overlay widget after creating it*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;
	
};
