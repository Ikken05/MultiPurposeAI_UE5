// Fill out your copyright notice in the Description page of Project Settings.

#include "RunBehaviorTreeFromBB.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

URunBehaviorTreeFromBB::URunBehaviorTreeFromBB()
{
	NodeName = "Run Behavior Tree from Blackboard";
}

EBTNodeResult::Type URunBehaviorTreeFromBB::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{// Get the Blackboard component
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        UE_LOG(LogTemp, Error, TEXT("Blackboard component not found"));
        return EBTNodeResult::Failed;
    }

    // Retrieve the Behavior Tree object from the Blackboard
    UObject* BehaviorTreeObj = BlackboardComp->GetValueAsObject(BehaviorTreeKey.SelectedKeyName);
    if (!BehaviorTreeObj)
    {
        UE_LOG(LogTemp, Error, TEXT("Behavior Tree object not found in Blackboard"));
        return EBTNodeResult::Failed;
    }

    // Cast the object to a UBehaviorTree
    UBehaviorTree* BehaviorTree = Cast<UBehaviorTree>(BehaviorTreeObj);
    if (!BehaviorTree)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast Blackboard value to UBehaviorTree"));
        return EBTNodeResult::Failed;
    }

    // Get the AIController to run the Behavior Tree
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController not found"));
        return EBTNodeResult::Failed;
    }

    // Run the Behavior Tree
    AIController->RunBehaviorTree(BehaviorTree);
    UE_LOG(LogTemp, Log, TEXT("Successfully ran Behavior Tree: %s"), *BehaviorTree->GetName());

    // Indicate success
    return EBTNodeResult::Succeeded;
}
