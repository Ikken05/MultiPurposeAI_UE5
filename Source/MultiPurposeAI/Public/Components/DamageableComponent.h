// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Damageable.h"
#include "DamageableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, DeadActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPURPOSEAI_API UDamageableComponent : public UActorComponent, public IDamageable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
    float CharacterCurrentHealth;
    float CharacterMaxHealth;

public:
    // Implement the interface functions
    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual float GetCurrentHealth() const override;
    UFUNCTION(BlueprintCallable, BlueprintPure)
    virtual float GetMaxHealth() const override;
    UFUNCTION(BlueprintCallable)
    virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
        AController* InstigatedBy, AActor* DamageCauser) override;
    virtual void SetMaxHealth(float MaxHealth) override;
    virtual void SetCurrentHealth(float Health) override;

public:
    // **Death Delegate**
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDeath OnDeath;
		
};
