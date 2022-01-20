// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->Controller);

	// Pawn that died was the player
	if (PlayerController)
	{
		EndGame(false);
		return;
	}

	// Pawn that died was an AI
	for (AShooterAIController* CurrentController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!CurrentController->IsDead())
		{
			return;
		}
	}

	// There is no more enemies alive
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* CurrentController : TActorRange<AController>(GetWorld()))
	{
		if (CurrentController->IsPlayerController())
		{
			CurrentController->GameHasEnded(CurrentController->GetPawn(), bIsPlayerWinner);
		}
		else
		{
			CurrentController->GameHasEnded(nullptr, !bIsPlayerWinner);
		}
	}
}