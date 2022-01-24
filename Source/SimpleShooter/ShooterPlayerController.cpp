// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDClass)
	{
		PlayerHUD = CreateWidget(this, PlayerHUDClass, TEXT("Player HUD"));
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport();
		}
	}
	if (ReloadHUDClass)
	{
		ReloadHUD = CreateWidget(this, ReloadHUDClass, TEXT("Reload HUD"));
	}
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	PlayerHUD->RemoveFromViewport();

	if (bIsWinner)
	{
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass, TEXT("Win Screen"));
		if (WinScreen)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass, TEXT("Lose Screen"));
		if (LoseScreen)
		{
			LoseScreen->AddToViewport();
		}
	}

	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}

void AShooterPlayerController::ShowReloadingHUD()
{
	if (ReloadHUD)
	{
		ReloadHUD->AddToViewport();
	}
}

void AShooterPlayerController::HideReloadingHUD()
{
	if (ReloadHUD)
	{
		ReloadHUD->RemoveFromViewport();
	}
}