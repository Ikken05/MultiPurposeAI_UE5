// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
#include "Components/ActorComponent.h"
#include "StateManagerComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class MULTIPURPOSEAI_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateManagerComponent();

private:

	EAICharacterState CurrentState;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EAICharacterState GetCurrentState() { return CurrentState; }
		
	UFUNCTION()
	void SetCurrentState(EAICharacterState NewState);
};
