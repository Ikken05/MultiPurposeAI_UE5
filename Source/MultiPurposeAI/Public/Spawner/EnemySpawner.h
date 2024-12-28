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
USTRUCT(BlueprintType)
struct FEnemySpawnData
{
    GENERATED_BODY()

    // Reference to the enemy's data asset
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Data")
    UCharacterDataAsset* EnemyDataAsset;

    // Transform for the enemy's spawn location, rotation, and scale
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FTransform SpawnTransform;

    //Visual Debug
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float DebugSphereRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    int32 NumberOfSegments;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    FColor SphereColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bPersistenLine;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    float Duration;


};


UCLASS(Blueprintable)
class MULTIPURPOSEAI_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
    // Constructor
    AEnemySpawner();

    // Method to spawn an enemy character based on the given data asset
    UFUNCTION(BlueprintCallable, Category = "Spawner")
    ACharacter* SpawnEnemy(const FEnemySpawnData& EnemyData);


public:

    UPROPERTY(EditAnywhere, Category= "Spawn|Enemies")
    TArray<FEnemySpawnData> EnemiesToSpawn;

#if WITH_EDITORONLY_DATA
    // Root component for editor visualization
    UPROPERTY(EditAnywhere, Category = "Spawner")
    USceneComponent* Root;

    // Optional gizmo for visualization
    UPROPERTY(EditAnywhere, Category = "Spawner")
    bool bShowSpawnPoints = true;
#endif

protected:

    // Helper methods for initializing enemy with specific assets
    UFUNCTION(BlueprintCallable, Category ="Init")
    void InitializeEnemy(ACharacter* SpawnedCharacter, const UCharacterDataAsset* CharacterDataAsset);

    // To add mapping context
    virtual void BeginPlay();

    // Property to store the default skeletal mesh for enemies
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    USkeletalMesh* DefaultSkeletalMesh;

    // Property to store the default animation blueprint for enemies
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    TSubclassOf<UAnimInstance> DefaultAnimBlueprint;

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

private:

    UFUNCTION(BlueprintCallable, Category = "AI|SubTrees")
    void AssignSubtrees(ACharacter* SpawnedEnemy);

	
};
