// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	UE_LOG(LogTemp, Warning, TEXT("A pawn was killed!"));

	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->Controller);
	if (PlayerController)
	{
		EndGame(false);
	}
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