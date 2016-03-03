// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/PlayerController.h"
#include "Unit.h"
#include <list>
#include "FinalProjectPlayerController.generated.h"

class AUnit;
/** PlayerController class used to enable cursor */
UCLASS()
class AFinalProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFinalProjectPlayerController();
    std::list<AUnit*> unitList;
    
};


