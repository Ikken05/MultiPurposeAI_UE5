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
ACharacter* AEnemySpawner::SpawnEnemy(const FEnemySpawnData& EnemyData)
{
    if (!EnemyData.EnemyDataAsset)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy data asset is null! Skipping spawn."));
        return nullptr;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // Spawn the enemy
    ACharacter* SpawnedCharacter = World->SpawnActor<ACharacter>(
        ACharacter::StaticClass(), // Replace with your base AI character class
        EnemyData.SpawnTransform);

    if (SpawnedCharacter)
    {
        // Initialize the spawned character with its data asset
        InitializeEnemy(SpawnedCharacter, EnemyData.EnemyDataAsset);
    }

    SpawnedCharacter->SetActorLocation(EnemyData.SpawnTransform.GetLocation(), false, nullptr, ETeleportType::None);
    FRotator SpawnRotation = EnemyData.SpawnTransform.GetRotation().Rotator();
    SpawnedCharacter->SetActorRotation(SpawnRotation);

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
    for (const FEnemySpawnData& EnemyData : EnemiesToSpawn)
    {
        SpawnEnemy(EnemyData);
    }
}

void AEnemySpawner::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (bShowSpawnPoints)
    {
        // Clear existing debug spheres
        FlushPersistentDebugLines(GetWorld());

        for (const FEnemySpawnData& EnemyData : EnemiesToSpawn)
        {
            if (EnemyData.SpawnTransform.GetLocation() != FVector::ZeroVector)
            {
                DrawDebugSphere(
                    GetWorld(),
                    EnemyData.SpawnTransform.GetLocation(),
                    EnemyData.DebugSphereRadius,
                    EnemyData.NumberOfSegments,
                    EnemyData.SphereColor,
                    EnemyData.bPersistenLine,
                    EnemyData.Duration      
                );
                // Draw a debug arrow to indicate the rotation
                FVector Start = EnemyData.SpawnTransform.GetLocation();
                FVector ForwardVector = EnemyData.SpawnTransform.GetRotation().GetRightVector();
                FVector ArrowEnd = Start + (ForwardVector * 100.0f);
                DrawDebugDirectionalArrow(
                    GetWorld(),
                    EnemyData.SpawnTransform.GetLocation(),
                    ArrowEnd,
                    50.0f,       // Arrow size
                    FColor::Blue, // Color
                    true,         // Persistent
                    0.0f,         // Never expires
                    0,            // Depth priority
                    2.0f          // Arrow thickness
                );
            }
        }
    }
}

void AEnemySpawner::AssignSubtrees(ACharacter* SpawnedEnemy)
{
}
