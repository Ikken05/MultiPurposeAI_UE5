// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Data/CharacterDataAsset.h"

// Constructor implementation
AEnemySpawner::AEnemySpawner()
{
    // Default values, can be changed in the editor or on spawn
    DefaultSkeletalMesh = nullptr;
    DefaultAnimBlueprint = nullptr;
}

// Function to spawn an enemy and assign assets based on the data asset provided
ACharacter* AEnemySpawner::SpawnEnemy(const UCharacterDataAsset* CharacterDataAsset)
{
    if (!CharacterDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("Character Data Asset is invalid!"));
        return nullptr;
    }

    // Spawn the enemy character from the provided class
    ACharacter* SpawnedCharacter = GetWorld()->SpawnActor<ACharacter>(CharacterDataAsset->CharacterClass);

    FVector SpawnPoint = GetActorLocation();

    SpawnedCharacter->SetActorLocation(SpawnPoint,false,nullptr,ETeleportType::None);

    if (SpawnedCharacter)
    {
        // Initialize the enemy with specific assets and behavior tree
        InitializeEnemy(SpawnedCharacter, CharacterDataAsset);
    }

    return SpawnedCharacter;
}

// Function to initialize the enemy's mesh, animation blueprint, and behavior tree
void AEnemySpawner::InitializeEnemy(ACharacter* SpawnedCharacter, const UCharacterDataAsset* CharacterDataAsset)
{
    if (SpawnedCharacter)
    {
        // Set the character's mesh and animation blueprint
        if (CharacterDataAsset->CharacterMesh)
        {
            SpawnedCharacter->GetMesh()->SetSkeletalMesh(CharacterDataAsset->CharacterMesh);
        }

        if (CharacterDataAsset->AnimationBlueprint)
        {
            SpawnedCharacter->GetMesh()->SetAnimInstanceClass(CharacterDataAsset->AnimationBlueprint);
        }
    }
}

void AEnemySpawner::BeginPlay()
{
    if(!EnemyDataAsset)
        return;
    SpawnEnemy(EnemyDataAsset);
}
