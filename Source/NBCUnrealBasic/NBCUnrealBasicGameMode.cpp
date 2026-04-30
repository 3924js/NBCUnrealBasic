// Copyright Epic Games, Inc. All Rights Reserved.

#include "NBCUnrealBasicGameMode.h"
#include "NBCUnrealBasicCharacter.h"
#include "UObject/ConstructorHelpers.h"

ANBCUnrealBasicGameMode::ANBCUnrealBasicGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
