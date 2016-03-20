// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "Unit.h"
#include "Engine.h"
#include "FinalProjectBlock.generated.h"

class AFinalProjectBlockGrid;
class AUnit;
/** A block that can be clicked */
UCLASS(minimalapi)
class AFinalProjectBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	/* Gird instace*/
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AFinalProjectBlockGrid* BlockGrid;

public:
    
    //unit currently occupying this block
    AUnit* unit;
    int row;
    int column;
    float locationZ;
    float sizeZ;
    bool clear; //if the node is occupied by a unit
    bool selected; //for placing units, only one block should be selected at any time.
	AFinalProjectBlock();
    void setUnit(AUnit* unit);
    FVector BlockLocation;
	/** Are we currently active? */
	bool bIsActive;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Pointer to Blue material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;


	/** Grid that owns us */
	UPROPERTY()
	class AFinalProjectBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp);

	UFUNCTION()
	void Unselect();

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
	/* Returns Grid object*/
	FORCEINLINE class AFinalProjectBlockGrid* GetGridRef() const { return OwningGrid; }
};



