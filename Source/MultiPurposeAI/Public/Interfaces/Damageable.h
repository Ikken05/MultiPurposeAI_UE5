// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MULTIPURPOSEAI_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	
	virtual float GetCurrentHealth() const = 0;
	virtual float GetMaxHealth() const = 0;
	virtual void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
		AController* InstigatedBy, AActor* DamageCauser) = 0;
	virtual void SetMaxHealth(float MaxHealth) = 0;
	virtual void SetCurrentHealth(float Health) = 0;

};
