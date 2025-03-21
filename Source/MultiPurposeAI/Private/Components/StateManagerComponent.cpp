// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StateManagerComponent.h"

// Sets default values for this component's properties
UStateManagerComponent::UStateManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStateManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStateManagerComponent::SetCurrentState(EAICharacterState NewState)
{
	CurrentState = NewState;
}


