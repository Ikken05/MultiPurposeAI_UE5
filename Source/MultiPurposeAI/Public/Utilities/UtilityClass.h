// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UtilityClass.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAICharacterState : uint8
{
    None        UMETA(DisplayName = "None"),
    Patrol      UMETA(DisplayName = "Patrol"),
    Attack      UMETA(DisplayName = "Attack"),
    Support     UMETA(DisplayName = "Support"),
    Standby     UMETA(DisplayName = "Standby")
};


UCLASS()
class MULTIPURPOSEAI_API UUtilityClass : public UObject
{
	GENERATED_BODY()
	
};
