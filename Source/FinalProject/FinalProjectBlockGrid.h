// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "FinalProjectBlock.h"
#include <list>
#include "FinalProjectBlockGrid.generated.h"

class AUnit;
class AFinalProjectBlock;
class APlayerInfo;
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
    
	APlayerInfo* player1;
	APlayerInfo* player2;
	APlayerInfo* activePlayer;

    AFinalProjectBlock*** grid;
    
	AFinalProjectBlockGrid();

	/** How many blocks have been clicked */
	int32 Score;
    
    int rows;
    int columns;
    AFinalProjectBlock* selectedBlock;
    
    //list of units
    std::list<AUnit*> unitList;
	std::list<AUnit*> killList;
    
	bool legalPlace();

	

    //gets the distance between two nodes
    int getDistance(AFinalProjectBlock* node1, AFinalProjectBlock* node2);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Grid)
    FString selectedUnitType;
    
	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	UPROPERTY(Category = Grid, EditAnywhere, BlueprintReadOnly)
		int32 playerInt;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	/** Handle the block being clicked */
	UFUNCTION(BlueprintCallable, Category = "Blocks")
	void SetActive(int32 r, int32 c);

	UFUNCTION(BlueprintCallable, Category = "Blocks")
	void setUnit(int32 u);
    
    UFUNCTION(BlueprintCallable, Category = "Units")
    void moveUnits();
    
	UFUNCTION(BlueprintCallable, Category = "Pl4yer")
	void endTurn();

	UFUNCTION(BlueprintCallable, Category = "Pl4yer")
	void enableAI();

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



