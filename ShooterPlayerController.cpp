// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"


AShooterPlayerController::AShooterPlayerController()
{

}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Check our HUDoverlayClass TSubclass of varibale
    // Set in the blueprint in the engine
    // if set proceed to create widget
    if (HUDOverlayClass)
    {
        // Creates widget and sets to HUDOverlay
        HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
        if (HUDOverlay)
        {
            HUDOverlay->AddToViewport();
            HUDOverlay->SetVisibility(ESlateVisibility::Visible);
        }
    }
}
