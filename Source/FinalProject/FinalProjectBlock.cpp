// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectBlock.h"
#include "FinalProjectBlockGrid.h"

AFinalProjectBlock::AFinalProjectBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.01f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AFinalProjectBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AFinalProjectBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
    this->unit = NULL;
    this->row = 0;
    this->column = 0;
    this->clear = true;
    this->selected = false;
}

void AFinalProjectBlock::BlockClicked(UPrimitiveComponent* ClickedComp)
{
	// Check we are not already active
	if(!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, OrangeMaterial);

		// Tell the Grid
		if(OwningGrid != NULL)
		{
			OwningGrid->SetActive(row, column);
		}
	}
	else {
		bIsActive = false;

		// Change material
		BlockMesh->SetMaterial(0, BlueMaterial);


	}
}

void AFinalProjectBlock::Unselect(){
	bIsActive = false;
	BlockMesh->SetMaterial(0, BlueMaterial);
}

void AFinalProjectBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent);
}

void AFinalProjectBlock::setUnit(AUnit* unit)
{
    this->unit = unit;
    this->clear = false;
    //TO DO - set the location of the unit based on the location of the block
}
