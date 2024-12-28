// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" 
#include "Enums.h"
#include "CharacterDataAsset.generated.h"





class UBehaviorTree;
class USkeletalMesh;
class UAnimInstance;

/**
 * Data Asset for AI Character Settings
 * Allows to setup the visuals of the enemy as well as its behavior via subtrees
 */


UCLASS(BlueprintType)
class MULTIPURPOSEAI_API UCharacterDataAsset : public UDataAsset
{
    GENERATED_BODY()

public:

    // This is the Character Class that will be spawned
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn")
    TSubclassOf<ACharacter> CharacterClass;

    //The Skeletal Mesh that will be assigned to the Spawned Character Class
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    USkeletalMesh* CharacterMesh;

    //The Animation Blueprint that will be used to animate the Spawned Character
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    TSubclassOf<UAnimInstance> AnimationBlueprint;

    // Subtrees mapping for different behaviors
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Behavior|Trees")
    TMap<EAICharacterState, UBehaviorTree*> Subtrees;
};
