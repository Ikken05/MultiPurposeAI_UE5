// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RunBehaviorTreeFromBB.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPURPOSEAI_API URunBehaviorTreeFromBB : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

    URunBehaviorTreeFromBB();

protected:
    /** The Blackboard key that contains the Behavior Tree to run */
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector BehaviorTreeKey;

    /** Executes the task */
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
