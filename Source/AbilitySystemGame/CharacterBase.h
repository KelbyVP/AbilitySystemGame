// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEMGAME_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		UAbilitySystemComponent* AbilitySystemComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		UAttributeSetBase* AttributeSetBaseComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CharacterBase")
		FGameplayTag FullHealthTag;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire);

	UFUNCTION()
		void OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnHealthChanged"))
		void BP_OnHealthChanged(float Health, float MaxHealth);

	UFUNCTION()
		void OnManaChanged(float Mana, float MaxMana);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnManaChanged"))
		void BP_OnManaChanged(float Mana, float MaxMana);

	UFUNCTION()
		void OnStrengthChanged(float Strength, float MaxStrength);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (DisplayName = "OnStrengthChanged"))
		void BP_OnStrengthChanged(float Strength, float MaxStrength);

	UFUNCTION(BlueprintImplementableEvent, Category = "CharacterBase", meta = (Display = "Die"))
		void BP_Die();

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		bool IsOtherHostile(ACharacterBase* Other);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void AddGameplayTag(FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "CharacterBase")
		void RemoveGameplayTag(FGameplayTag TagToRemove);

	uint8 GetTeamID() const;

	protected:

	bool bIsDead;
	uint8 TeamID;
	void AutoDetermineTeamIDByControllerType();
	void OnDeath();

};
