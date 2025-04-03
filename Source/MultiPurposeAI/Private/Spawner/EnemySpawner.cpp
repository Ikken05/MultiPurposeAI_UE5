// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIController.h"
#include "Enums.h"
#include "Components/StateManagerComponent.h"
#include "Components/DamageableComponent.h"
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

    AddComponentsToCharacter(CharacterDataAsset, SpawnedCharacter);

    bool bSuccess = AICharacterController->RunBehaviorTree(CharacterDataAsset->MainBT);
    if(bSuccess)
    {
        UBlackboardComponent* BlackboardComp = AICharacterController->GetBlackboardComponent();
        if (BlackboardComp)
        {
            // Initialize AIState for EAICharacterState
            UEnum* AIState = StaticEnum<EAICharacterState>();

            for (const auto& Pair : CharacterDataAsset->Subtrees)
            {
                EAICharacterState State = Pair.Key;         // Key: AI character state
                UBehaviorTree* Subtree = Pair.Value;       // Value: Behavior tree

                if (Subtree)
                {
                    // Get the display name of the enum value (e.g., "Patrol" instead of "EAICharacterState::Patrol")
                    FString StateString = *AIState->GetNameStringByValue((int64)State);
                    // Create the blackboard key with the format EnumName + "SubTree"
                    FString BlackboardKeyString = StateString + "SubTree";
                    FName BlackboardKey = FName(*BlackboardKeyString);

                    // Set the behavior tree as an object in the blackboard
                    BlackboardComp->SetValueAsObject(BlackboardKey, Subtree);
                }
            }
            UStateManagerComponent* StateManager = SpawnedCharacter->GetComponentByClass<UStateManagerComponent>();
            if(StateManager)
            {
                StateManager->SetCurrentState(CharacterDataAsset->DefaultStartState);
            }

            UDamageableComponent* DamageComponent = SpawnedCharacter->GetComponentByClass<UDamageableComponent>();
            if (DamageComponent)
            {
                DamageComponent->SetMaxHealth(CharacterDataAsset->MaxHealth);
                DamageComponent->SetCurrentHealth(DamageComponent->GetMaxHealth());
            }

            FName AIStateBlackboardKey = FName("AIState");
            BlackboardComp->SetValueAsEnum(AIStateBlackboardKey, static_cast<uint8>(CharacterDataAsset->DefaultStartState));

        }
    }
}

void AEnemySpawner::AddComponentsToCharacter(const UCharacterDataAsset* CharacterDataAsset, ACharacter* SpawnedEnemy)
{
    if (!SpawnedEnemy || !CharacterDataAsset)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid SpawnedEnemy or CharacterDataAsset!"));
        return;
    }

    if (CharacterDataAsset->bEnableComponents)
    {
        UE_LOG(LogTemp, Warning, TEXT("Adding components to character: %s"), *SpawnedEnemy->GetName());
        UE_LOG(LogTemp, Warning, TEXT("ComponentsToAdd has %d elements"), CharacterDataAsset->ComponentsToAdd.Num());

        for (const TSubclassOf<UActorComponent>& CompClass : CharacterDataAsset->ComponentsToAdd)
        {
            if (!CompClass || !CompClass->IsChildOf(UActorComponent::StaticClass()))
            {
                UE_LOG(LogTemp, Error, TEXT("Invalid component class in ComponentsToAdd!"));
                continue;
            }

            // Create the new Actor Component
            UActorComponent* NewComponent = NewObject<UActorComponent>(SpawnedEnemy, CompClass);

            if (NewComponent)
            {
                UE_LOG(LogTemp, Warning, TEXT("Successfully created component: %s"), *NewComponent->GetName());

                // Register the component with the actor
                NewComponent->RegisterComponent();
                NewComponent->SetActive(true);

                UE_LOG(LogTemp, Warning, TEXT("Registered and activated component: %s"), *NewComponent->GetName());

                // Attach the component to the actor
                SpawnedEnemy->AddOwnedComponent(NewComponent);  // Add it to the actor's owned components
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("bEnableComponents is false, no components added."));
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
        PerceptionComponent->RegisterComponent();

        if (PerceptionComponent)
        {
            AICharacterController->SetPerceptionComponent(*PerceptionComponent);
        }
    }

    if (PerceptionComponent)
    {
        

        // Configure senses from data asset
        for (const TObjectPtr<UAISenseConfig>& SenseConfig : CharacterDataAsset->SensesConfig)
        {
            if (SenseConfig)
            {
                UE_LOG(LogTemp, Error, TEXT("Sense: %s"), *SenseConfig->GetName());
                
                PerceptionComponent->ConfigureSense( *SenseConfig);
            }
        }
        
        
        // Set dominant sense
        if (CharacterDataAsset->DominantSense)
        {
            PerceptionComponent->SetDominantSense(CharacterDataAsset->DominantSense);
            PerceptionComponent->RequestStimuliListenerUpdate();
        }  
        // Ensure activation
        PerceptionComponent->Activate();
        // Add dynamic delegate for perception updates
        PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemySpawner::OnPerceptionUpdated);
        if (PerceptionComponent->OnPerceptionUpdated.IsBound())
        {
            UE_LOG(LogTemp, Log, TEXT("Perception delegate successfully bound."));
        }
    }
}


void AEnemySpawner::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    UE_LOG(LogTemp, Warning, TEXT("Perception Updated Called"));
    bool bPlayerDetected = false;
    AActor* DetectedActor = nullptr;
    for (AActor* PerceivedActor : UpdatedActors)
    {
        if (!PerceivedActor) continue;

        for (ACharacter* Enemy : SpawnedEnemies)
        {
            if (!Enemy) continue;
            bPlayerDetected = HandlePerceptionForEnemy(Enemy, PerceivedActor);
            DetectedActor = PerceivedActor;
        }
    }
    if (bPlayerDetected)
    {
        for (ACharacter* Enemy : SpawnedEnemies)
        {
            int32 Index = SpawnedEnemies.IndexOfByKey(Enemy);
            if (Index == INDEX_NONE || !EnemiesToSpawn.IsValidIndex(Index)) return;

            FEnemySpawnData* EnemyData = &EnemiesToSpawn[Index];
            if (!EnemyData) return;

            AAIController* AIController = Cast<AAIController>(Enemy->GetController());
            if (!AIController) return;

            UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
            if (!BlackBoard) return;

            FName AIStateBlackboardKey = FName("AIState");
            FName TargetActorName = FName("TargetActor");

            BlackBoard->SetValueAsEnum(AIStateBlackboardKey, static_cast<uint8>(EnemyData->EnemyDataAsset->DetectionState));
            BlackBoard->SetValueAsObject(TargetActorName, DetectedActor);
            BlackBoard->SetValueAsVector(FName("TargetLocation"), DetectedActor->GetActorLocation());

            AIController->RunBehaviorTree(EnemyData->EnemyDataAsset->MainBT);

            if (EnemyData->EnemyDataAsset->bEnableComponents)
            {
                UStateManagerComponent* StateManager = Enemy->GetComponentByClass<UStateManagerComponent>();
                if (StateManager)
                    StateManager->SetCurrentState(EnemyData->EnemyDataAsset->DetectionState);
            }
        }
    }

}

bool AEnemySpawner::HandlePerceptionForEnemy(ACharacter* Enemy, AActor* PerceivedActor)
{
    AAIController* AIController = Cast<AAIController>(Enemy->GetController());
    if (!AIController) return false;

    UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
    if (!BlackBoard) return false;

    UAIPerceptionComponent* PerceptionComponent = AIController->FindComponentByClass<UAIPerceptionComponent>();
    if (!PerceptionComponent) return false;

    FActorPerceptionBlueprintInfo ActorInfo;
    PerceptionComponent->GetActorsPerception(PerceivedActor, ActorInfo);

    for (const FAIStimulus& Stimulus : ActorInfo.LastSensedStimuli)
    {
        TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);
        if (!SenseClass) continue;

        FString SenseType = SenseClass->GetName();
        UE_LOG(LogTemp, Warning, TEXT("Enemy %s detected %s with sense: %s"), *Enemy->GetName(), *PerceivedActor->GetName(), *SenseType);

        return DetermineEnemyReaction(Enemy, PerceivedActor, SenseType);
    }
    return true;
}

bool AEnemySpawner::DetermineEnemyReaction(ACharacter* Enemy, AActor* PerceivedActor, const FString& SenseType)
{
    bool bPlayerDetected = false;
    int32 Index = SpawnedEnemies.IndexOfByKey(Enemy);
    if (Index == INDEX_NONE || !EnemiesToSpawn.IsValidIndex(Index)) return false;

    FEnemySpawnData* EnemyData = &EnemiesToSpawn[Index];
    if (!EnemyData) return false;

    AAIController* AIController = Cast<AAIController>(Enemy->GetController());
    if (!AIController) return false;

    UBlackboardComponent* BlackBoard = AIController->GetBlackboardComponent();
    if (!BlackBoard) return false;

    FName AIStateBlackboardKey = FName("AIState");
    FName TargetActorName = FName("TargetActor");

    if (SenseType == "AISense_Sight"|| SenseType == "AISense_Hearing" || SenseType == "AISense_Damage")
    {
        BlackBoard->SetValueAsEnum(AIStateBlackboardKey, static_cast<uint8>(EnemyData->EnemyDataAsset->DetectionState));
        BlackBoard->SetValueAsObject(TargetActorName, PerceivedActor);

        if (EnemyData->EnemyDataAsset->bEnableComponents)
        {
            UStateManagerComponent* StateManager = Enemy->GetComponentByClass<UStateManagerComponent>();
            if (StateManager)
                StateManager->SetCurrentState(EnemyData->EnemyDataAsset->DetectionState);
        }
        return true;
    }
    return true;
}




