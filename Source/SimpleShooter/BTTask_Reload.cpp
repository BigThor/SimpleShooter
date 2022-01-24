// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Reload.h"
#include "AIController.h"
#include "ShooterCharacter.h"

UBTTask_Reload::UBTTask_Reload()
{
	NodeName = TEXT("Reload");
}

EBTNodeResult::Type UBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	AShooterCharacter* AICharacter = Cast<AShooterCharacter>(AIController->GetPawn());
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	AICharacter->StartReload();
	return EBTNodeResult::Succeeded;
}