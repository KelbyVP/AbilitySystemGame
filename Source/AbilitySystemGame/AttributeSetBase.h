// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameFramework/Actor.h"
#include "AttributeSetBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChange, float, Health, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChange, float, Mana, float, MaxMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrengthChange, float, Strength, float, MaxStrength);

UCLASS()
class ABILITYSYSTEMGAME_API UAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	
public:
	UAttributeSetBase();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData Mana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData MaxMana;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData Strength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData MaxStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData AttackDamage;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttributeBase")
		FGameplayAttributeData Armor;

	void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	FOnHealthChange OnHealthChange;
	FOnManaChange OnManaChange;
	FOnStrengthChange OnStrengthChange;

};
