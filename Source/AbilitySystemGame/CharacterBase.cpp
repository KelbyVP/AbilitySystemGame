// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterBase.h"
#include "AttributeSetBase.h"
#include "BrainComponent.h"
#include "GameFramework/Playercontroller.h"
#include "AIController.h"
#include "PlayerControllerBase.h"
#include "AbilityTypes.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComp = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComp");
	AttributeSetBaseComp = CreateDefaultSubobject<UAttributeSetBase>("AttributeSetBase");
	bIsDead = false;
	TeamID = 255;
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	AttributeSetBaseComp->OnHealthChange.AddDynamic(this, &ACharacterBase::OnHealthChanged);
	AttributeSetBaseComp->OnManaChange.AddDynamic(this, &ACharacterBase::OnManaChanged);
	AttributeSetBaseComp->OnStrengthChange.AddDynamic(this, &ACharacterBase::OnStrengthChanged);
	AutoDetermineTeamIDByControllerType();
	AddGameplayTag(FullHealthTag);
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent * ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

void ACharacterBase::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAcquire) {
	if (AbilitySystemComp) 
	{
		if (HasAuthority() && AbilityToAcquire) 
		{
			AbilitySystemComp->GiveAbility(FGameplayAbilitySpec(AbilityToAcquire));
		}
		AbilitySystemComp->InitAbilityActorInfo(this, this);
	}
}

void ACharacterBase::AcquireAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToAcquire)
{
	for (TSubclassOf<UGameplayAbility> AbilityItem : AbilitiesToAcquire)
	{
		AcquireAbility(AbilityItem);
		if (AbilityItem->IsChildOf(UGameplayAbilityBase::StaticClass()))
		{
			TSubclassOf<UGameplayAbilityBase> AbilityBaseClass = *AbilityItem;
			if (AbilityBaseClass != nullptr)
			{
				AddAbilityToUI(AbilityBaseClass);
			}
		}
	}
}

void ACharacterBase::OnHealthChanged(float Health, float MaxHealth)
{
	if (Health <= 0.0f && !bIsDead)
	{
		OnDeath();
		bIsDead = true;
		BP_Die();
	}
	BP_OnHealthChanged(Health, MaxHealth);
}

void ACharacterBase::OnManaChanged(float Mana, float MaxMana)
{
	BP_OnManaChanged(Mana, MaxMana);
}

void ACharacterBase::OnStrengthChanged(float Strength, float MaxStrength)
{
	BP_OnStrengthChanged(Strength, MaxStrength);
}


void ACharacterBase::RemoveGameplayTag(FGameplayTag TagToRemove)
{
	GetAbilitySystemComponent()->RemoveLooseGameplayTag(TagToRemove);
}

void ACharacterBase::HitStun(float StunDuration)
{
	DisableInputControl();
	GetWorldTimerManager().SetTimer(StunTimeHandle, this, &ACharacterBase::EnableInputControl, StunDuration, false);
}

uint8 ACharacterBase::GetTeamID() const
{
	return TeamID;
}

void ACharacterBase::AutoDetermineTeamIDByControllerType()
{
	if (GetController()->IsPlayerController())
	{
		TeamID = 0;
	}
}

void ACharacterBase::OnDeath()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->DisableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Dead");
	}
}

void ACharacterBase::DisableInputControl()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->DisableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->StopLogic("Dead");
	}
}

void ACharacterBase::EnableInputControl()
{
	if (!bIsDead)
	{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		PC->EnableInput(PC);
	}
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		AIC->GetBrainComponent()->RestartLogic();
	}
	}
}

void ACharacterBase::AddAbilityToUI(TSubclassOf<UGameplayAbilityBase> AbilityToAdd)
{
	APlayerControllerBase* PlayerControllerBase = Cast<APlayerControllerBase>(GetController());
	if (PlayerControllerBase)
	{
		UGameplayAbilityBase *AbilityInstance = AbilityToAdd.Get()->GetDefaultObject<UGameplayAbilityBase>();
		if (AbilityInstance)
		{
			FGameplayAbilityInfo AbilityInfo = AbilityInstance->GetAbilityInfo();
			PlayerControllerBase->AddAbilityToUI(AbilityInfo);
		}
	}
}

bool ACharacterBase::IsOtherHostile(ACharacterBase* Other)
{
	return TeamID != Other->GetTeamID();
}

void ACharacterBase::AddGameplayTag(FGameplayTag TagToAdd)
{
	GetAbilitySystemComponent()->AddLooseGameplayTag(TagToAdd);
	GetAbilitySystemComponent()->SetTagMapCount(TagToAdd, 1);
}
