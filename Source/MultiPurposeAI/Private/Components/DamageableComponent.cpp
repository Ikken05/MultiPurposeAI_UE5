// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DamageableComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDamageableComponent::UDamageableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageableComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UDamageableComponent::TakeDamage);
		UE_LOG(LogTemp, Error, TEXT("Bound"));
	}
}

float UDamageableComponent::GetCurrentHealth() const
{
	return CharacterCurrentHealth;
}

float UDamageableComponent::GetMaxHealth() const
{
	return CharacterMaxHealth;
}

void UDamageableComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
		return;
	float Health;

	Health = FMath::Max(CharacterCurrentHealth - Damage, 0.0f);

	SetCurrentHealth(Health);

	UE_LOG(LogTemp, Warning, TEXT("%s's DamageableComponent took %f damage. Remaining health: %f"),
		*GetOwner()->GetName(), Damage, Health);

	if (CharacterCurrentHealth <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s died!"), *GetOwner()->GetName());
		OnDeath.Broadcast(GetOwner());
		GetOwner()->Destroy();
	}
}

void UDamageableComponent::SetMaxHealth(float MaxHealth)
{
	CharacterMaxHealth = MaxHealth;
}

void UDamageableComponent::SetCurrentHealth(float Health)
{
	CharacterCurrentHealth = Health;
}


