// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "BehaviorTree/BlackboardComponent.h" 
#include "Data/CharacterDataAsset.h"

// Constructor implementation
AEnemySpawner::AEnemySpawner()
{
    // Default values, can be changed in the editor or on spawn
    DefaultSkeletalMesh = nullptr;
    DefaultAnimBlueprint = nullptr;
}

void AEnemySpawner::BeginPlay()
{
    for (const FEnemySpawnData& EnemyData : EnemiesToSpawn)
    {
        ACharacter* SpawnedEnemy = SpawnEnemy(EnemyData);
        if (SpawnedEnemy)
        {

            SpawnedEnemies.Add(SpawnedEnemy);
        }

    }
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
        EnemyData.EnemyDataAsset->CharacterClass, 
        EnemyData.SpawnTransform);

    if (SpawnedCharacter)
    {
        AAIController* AIController = Cast<AAIController>(SpawnedCharacter->GetController());

        // If the character doesn't have a controller, create one
        if (!AIController)
        {
            // Spawn the default AI controller for the character
            SpawnedCharacter->SpawnDefaultController();

            // Get the newly created controller
            AIController = Cast<AAIController>(SpawnedCharacter->GetController());
        }

        // Initialize the enemy with the assigned AIController
        if (AIController)
        {
            InitializeEnemy(SpawnedCharacter, EnemyData.EnemyDataAsset, AIController);
            AssignAIPerceptionConfig(SpawnedCharacter, EnemyData.EnemyDataAsset, AIController);
        }
    }

    SpawnedCharacter->SetActorLocation(EnemyData.SpawnTransform.GetLocation(), false, nullptr, ETeleportType::None);
    FRotator SpawnRotation = EnemyData.SpawnTransform.GetRotation().Rotator();
    SpawnedCharacter->SetActorRotation(SpawnRotation);

    return SpawnedCharacter;
}

// Function to initialize the enemy's mesh, animation blueprint, and behavior tree
void AEnemySpawner::InitializeEnemy(ACharacter* SpawnedCharacter, const UCharacterDataAsset* CharacterDataAsset, AAIController* AICharacterController)
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



    bool bSuccess = AICharacterController->RunBehaviorTree(CharacterDataAsset->MainBT);
    if(bSuccess)
    {
        UBlackboardComponent* BlackboardComp = AICharacterController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            // Get the UEnum for EAICharacterState
            UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAICharacterState"), true);
            
            if (!EnumPtr)
            {
                UE_LOG(LogTemp, Error, TEXT("Enum EAICharacterState not found!"));
                return;
            }

            for (const auto& Pair : CharacterDataAsset->Subtrees)
            {
                EAICharacterState State = Pair.Key;         // Key: AI character state
                UBehaviorTree* Subtree = Pair.Value;       // Value: Behavior tree

                if (Subtree)
                {
                    // Get the display name of the enum value (e.g., "Patrol" instead of "EAICharacterState::Patrol")
                    FString StateString = EnumPtr->GetDisplayNameTextByValue((int64)State).ToString();
                    // Create the blackboard key with the format EnumName + "SubTree"
                    FString BlackboardKeyString = StateString + "SubTree";
                    FName BlackboardKey = FName(*BlackboardKeyString);

                    // Set the behavior tree as an object in the blackboard
                    BlackboardComp->SetValueAsObject(BlackboardKey, Subtree);
                }
            }
        }
    }
}





void AEnemySpawner::AssignAIPerceptionConfig(ACharacter* SpawnedCharacter, const UCharacterDataAsset* CharacterDataAsset, AAIController* AICharacterController)
{
    if (!SpawnedCharacter || !CharacterDataAsset || !AICharacterController)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid parameters in AssignAIPerceptionConfig"));
        return;
    }

    UAIPerceptionComponent* PerceptionComponent = AICharacterController->FindComponentByClass<UAIPerceptionComponent>();

    if (!PerceptionComponent)
    {
        // Create and register a new Perception Component at runtime
        PerceptionComponent = NewObject<UAIPerceptionComponent>(AICharacterController);
        if (PerceptionComponent)
        {
            PerceptionComponent->RegisterComponent();
            AICharacterController->SetPerceptionComponent(*PerceptionComponent);
        }
    }

    if (!PerceptionComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create or find PerceptionComponent!"));
        return;
    }

    // Configure Senses from Data Asset
    for (const TObjectPtr<UAISenseConfig>& SenseConfig : CharacterDataAsset->SensesConfig)
    {
        if (SenseConfig)
        {
            PerceptionComponent->ConfigureSense(*SenseConfig);
        }
    }

    // Set Dominant Sense
    if (CharacterDataAsset->DominantSense)
    {
        PerceptionComponent->SetDominantSense(CharacterDataAsset->DominantSense);
    }

    // Ensure activation
    PerceptionComponent->Activate();

    // Add dynamic delegate for perception updates
    PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemySpawner::OnPerceptionUpdated);
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
                FVector ForwardVector = EnemyData.SpawnTransform.GetRotation().Vector();
                FVector ArrowEnd = Start + (ForwardVector * EnemyData.ArrowLength);
                DrawDebugDirectionalArrow(
                    GetWorld(),
                    EnemyData.SpawnTransform.GetLocation(),
                    ArrowEnd,
                    EnemyData.DebugSphereRadius,       // Arrow size
                    EnemyData.SphereColor, // Color
                    EnemyData.bPersistenLine,        // Persistent
                    EnemyData.Duration,         // Never expires
                    0,            // Depth priority
                    EnemyData.ArrowThickness          // Arrow thickness
                );
            }
        }
    }
}

void AEnemySpawner::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    // Handle perception updates
    for (AActor* Actor : UpdatedActors)
    {
        if (Actor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Perceived Actor: %s"), *Actor->GetName());
        }
    }
}


