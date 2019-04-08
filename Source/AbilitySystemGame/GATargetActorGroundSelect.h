// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATargetActorGroundSelect.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMGAME_API AGATargetActorGroundSelect : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()


public:

		virtual void StartTargeting(UGameplayAbility* Ability) override;
		virtual void ConfirmTargetingAndContinue() override;
		virtual void Tick(float DeltaSeconds) override;

		UFUNCTION(BlueprintCallable, Category = "GroundSelect")
			bool GetPlayerLookingPoint(FVector& OutViewPoint);

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundSelect")
			float Radius;
	
};