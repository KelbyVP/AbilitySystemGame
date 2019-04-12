// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGetAround.h"
#include "GameFramework/PlayerController.h"
#include "GameplayAbility.h"
#include "GameFramework/Pawn.h"

AGATargetActorGetAround::AGATargetActorGetAround()
{

}

void AGATargetActorGetAround::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGetAround::ConfirmTargetingAndContinue()
{
	FVector MasterPawnLocation;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (!MasterPawn)
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
		return;
	}
	MasterPawnLocation = MasterPawn->GetActorLocation();

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	bool TraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;

	if (MasterPawn)
	{
		CollisionQueryParams.AddIgnoredActor(MasterPawn);
	}
	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
		MasterPawnLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		FCollisionShape::MakeSphere(Radius),
		CollisionQueryParams);
	if (TryOverlap)
	{
		for (const auto& o : Overlaps)
		{
			if (auto pawn = Cast<APawn>(o.GetActor()))
			{
				OverlappedActors.AddUnique(pawn);
			}
		}
	}

	FGameplayAbilityTargetData_LocationInfo *CenterLocation = new FGameplayAbilityTargetData_LocationInfo();

		CenterLocation->TargetLocation.LiteralTransform = MasterPawn->GetTransform();
		CenterLocation->TargetLocation.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;


	if (OverlappedActors.Num() > 0)
	{
		FGameplayAbilityTargetDataHandle TargetData = StartLocation.MakeTargetDataHandleFromActors(OverlappedActors);
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
}
