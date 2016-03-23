// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "FinalProjectBlock.h"
#include "FinalProjectBlockGrid.generated.h"

class AFinalProjectBlock;
/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AFinalProjectBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

	/* Gird instace*/
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AFinalProjectBlockGrid* BlockGrid;
	

public:
    //grid to hold the blocks
    //AFinalProjectBlock grid[][];
    
    AFinalProjectBlock*** grid;
    
	AFinalProjectBlockGrid();

	/** How many blocks have been clicked */
	int32 Score;
    
    int rows;
    int columns;
    AFinalProjectBlock* selectedBlock;
    
    //list of units
    //std::list<AUnit*> unitList;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Grid)
    FString selectedUnitType;
    
	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	/** Handle the block being clicked */
	void SetActive(int r, int c);

	UFUNCTION(BlueprintCallable, Category = "Blocks")
	void setUnit(int32 u);
    
    UFUNCTION(BlueprintCallable, Category = "Units")
    void moveUnits();
    
    //getters for the grid node and its neighbors
    AFinalProjectBlock* getNode(int row, int column);
    AFinalProjectBlock* getNorthNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getSouthNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getEastNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getWestNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getNorthEastNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getNorthWestNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getSouthEastNode(AFinalProjectBlock* node);
    AFinalProjectBlock* getSouthWestNode(AFinalProjectBlock* node);
    

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }
	/* Returns Grid object*/
	FORCEINLINE class AFinalProjectBlockGrid* GetGridRef() { return this; }

};



