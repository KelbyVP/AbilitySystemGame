// Fill out your copyright notice in the Description page of Project Settings.

#include "AttributeSetBase.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "Math/UnrealMathUtility.h"

UAttributeSetBase::UAttributeSetBase()
	:Health(200.0f),
	MaxHealth(200.0f)
{

}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	if (Data.EvaluatedData.Attribute.GetUProperty() == FindFieldChecked<UProperty>(UAttributeSetBase::StaticClass(),GET_MEMBER_NAME_CHECKED(UAttributeSetBase, Health)))
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.0f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.0f, MaxHealth.GetCurrentValue()));
		UE_LOG(LogTemp, Warning, TEXT("Ouch, I took some damage, no my health is %f"), Health.GetCurrentValue());
		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
}
