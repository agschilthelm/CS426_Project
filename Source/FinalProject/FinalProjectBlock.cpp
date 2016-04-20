// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectBlock.h"
#include "FinalProjectBlockGrid.h"

AFinalProjectBlock::AFinalProjectBlock()
{
    this->locationZ = 25.f;
    this->sizeZ = 0.01f;
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> defaultMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, defaultMaterial(TEXT("/Game/Puzzle/Meshes/M_Rock_Slate.M_Rock_Slate"))
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
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,sizeZ));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,locationZ));
	BlockMesh->SetMaterial(0, ConstructorStatics.defaultMaterial.Get());
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AFinalProjectBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AFinalProjectBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	defaultMaterial = ConstructorStatics.defaultMaterial.Get();
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
		BlockMesh->SetMaterial(0, defaultMaterial);


	}
}

void AFinalProjectBlock::Unselect(){
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("unselect"));
    UE_LOG(LogTemp, Warning, TEXT("DEBUG: Unselect Block") );
	bIsActive = false;
	BlockMesh->SetMaterial(0, defaultMaterial);
}

void AFinalProjectBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent);
}

void AFinalProjectBlock::setUnit(AUnit* unit)
{
    UE_LOG(LogTemp, Warning, TEXT("DEBUG: set unit in block") );
    this->unit = unit;
    this->clear = false;
    //TO DO - set the location of the unit based on the location of the block
}
