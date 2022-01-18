// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehaviorTree)
	{
		RunBehaviorTree(AIBehaviorTree);

		/*PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

		Blackboard->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());*/
	}
}

void AShooterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (LineOfSightTo(PlayerPawn))
	{
		Blackboard->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		Blackboard->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	}
	else
	{
		Blackboard->ClearValue(TEXT("PlayerLocation"));
	}*/
}