// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EAICharacterState : uint8
{
    None        UMETA(DisplayName = "None"),
    Patrol      UMETA(DisplayName = "Patrol"),
    Investigate UMETA(DisplayName = "Investigate"),
    Attack      UMETA(DisplayName = "Attack"),
    Support     UMETA(DisplayName = "Support"),
    StandBy     UMETA(DisplayName = "StandBy")
};

