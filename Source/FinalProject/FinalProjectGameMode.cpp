// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectGameMode.h"
#include <iostream>
#include "FinalProjectPlayerController.h"

AFinalProjectGameMode::AFinalProjectGameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = AFinalProjectPlayerController::StaticClass();
}

void AFinalProjectGameMode::endTurn()
{
    std::cout << "end turn!";
}
