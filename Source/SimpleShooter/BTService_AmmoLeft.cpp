// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_AmmoLeft.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AmmoLeft::UBTService_AmmoLeft()
{
	NodeName = TEXT("Update Ammo Left");
}

void UBTService_AmmoLeft::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return;
	}

	AShooterCharacter* AICharacter = Cast<AShooterCharacter>(AIController->GetPawn());
	if (!AICharacter)
	{
		return;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	Blackboard->SetValueAsInt(GetSelectedBlackboardKey(), AICharacter->GetAmmoLeft());
}