// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemGameGameMode.h"
#include "AbilitySystemGameHUD.h"
#include "AbilitySystemGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAbilitySystemGameGameMode::AAbilitySystemGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AAbilitySystemGameHUD::StaticClass();
}
