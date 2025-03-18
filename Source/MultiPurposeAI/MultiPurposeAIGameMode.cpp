// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPurposeAIGameMode.h"
#include "MultiPurposeAICharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiPurposeAIGameMode::AMultiPurposeAIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
