// Fill out your copyright notice in the Description page of Project Settings.

#include "GATargetActorGroundSelect.h"
#include "GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include <GameFramework/PlayerController.h>
#include "DrawDebugHelpers.h"

void AGATargetActorGroundSelect::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	MasterPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
}

void AGATargetActorGroundSelect::ConfirmTargetingAndContinue()
{
	FVector ViewLocation;
	GetPlayerLookingPoint(ViewLocation);

	TArray<FOverlapResult> Overlaps;
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	bool TraceComplex = false;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.bTraceComplex = TraceComplex;
	CollisionQueryParams.bReturnPhysicalMaterial = false;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		CollisionQueryParams.AddIgnoredActor(MasterPawn);
	}
	bool TryOverlap = GetWorld()->OverlapMultiByObjectType(Overlaps,
		ViewLocation, 
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



void AGATargetActorGroundSelect::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector LookingPoint;
	GetPlayerLookingPoint(LookingPoint);
	DrawDebugSphere(GetWorld(), LookingPoint, Radius, 32, FColor::Red, true, -1, 0, 5.0f);
}

bool AGATargetActorGroundSelect::GetPlayerLookingPoint(FVector& OutViewPoint)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	MasterPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = MasterPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}

	bool TryTrace = GetWorld()->LineTraceSingleByChannel(HitResult, ViewPoint, ViewPoint + ViewRotation.Vector()*10000.0f, ECC_Visibility, QueryParams);
	if (TryTrace)
	{
		OutViewPoint = HitResult.ImpactPoint;
	}
	else
	{
		OutViewPoint = FVector();
	}
	return TryTrace;
}
