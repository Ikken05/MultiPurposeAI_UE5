// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemySpawner.generated.h"



class ACharacter;
class UBehaviorTree;
class USkeletalMesh;
class UAnimInstance;
class UCharacterDataAsset;

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPURPOSEAI_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
    // Constructor
    AEnemySpawner();

    // Method to spawn an enemy character based on the given data asset
    ACharacter* SpawnEnemy(const UCharacterDataAsset* CharacterDataAsset);


public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    UCharacterDataAsset* EnemyDataAsset;

protected:
    // Helper methods for initializing enemy with specific assets
    void InitializeEnemy(ACharacter* SpawnedCharacter, const UCharacterDataAsset* CharacterDataAsset);

    // To add mapping context
    virtual void BeginPlay();

    // Property to store the default skeletal mesh for enemies
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    USkeletalMesh* DefaultSkeletalMesh;

    // Property to store the default animation blueprint for enemies
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    TSubclassOf<UAnimInstance> DefaultAnimBlueprint;

	
};
