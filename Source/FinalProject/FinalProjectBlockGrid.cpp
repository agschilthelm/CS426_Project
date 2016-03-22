// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectBlockGrid.h"
#include "Components/TextRenderComponent.h"
#include "Unit.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AFinalProjectBlockGrid::AFinalProjectBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->AttachTo(DummyRoot);

	// Set defaults
    rows = 8;
    columns = 8;
    Size = rows;
	BlockSpacing = 270.f;
    //grid[Size][Size];
    
    this->grid = new AFinalProjectBlock**[rows];
    for (int i = 0; i < rows; ++i)
        grid[i] = new AFinalProjectBlock*[columns];

}


void AFinalProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
    int cnt = 0;
    
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            const float XOffset = (cnt/Size) * BlockSpacing; // Divide by dimension
            const float YOffset = (cnt%Size) * BlockSpacing; // Modulo gives remainder
            cnt++;
            // Make postion vector, offset from Grid location
            const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();
            
            // Spawn a block
            AFinalProjectBlock* NewBlock = GetWorld()->SpawnActor<AFinalProjectBlock>(BlockLocation, FRotator(0,0,0));
            NewBlock->BlockLocation = BlockLocation;
            NewBlock->OwningGrid = this;
            // Tell the block about its owner
            if(NewBlock != NULL)
            {
                //NewBlock->OwningGrid = this;
                grid[i][j] = NewBlock;
                NewBlock->row = i;
                NewBlock->column = j;
            }
        }
    }

}

void AFinalProjectBlockGrid::SetActive(int r, int c)
{
    if(selectedBlock != NULL)
        selectedBlock->Unselect();
	selectedBlock = getNode(r,c);
}

void AFinalProjectBlockGrid::setUnit(){
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("setUnit"));
    
	if (selectedBlock == NULL || selectedBlock->clear == false) {
		return;
	}
	int u = 88;
	AUnit *unit;
    std::string type = "scout";
    int strength;
	switch (u) {
	case 0:
		//King
		//unit = new AUnit();
        type = "king";
        strength = 0;
	case 1:
		//Knight
		//unit = new AUnit("knight", selectedBlock, this);
        type = "knight";
        strength = 0;
	case 2:
		//Soldier
		//unit = new AUnit("soldier", selectedBlock, this);
        type = "soldier";
        strength = 0;
	case 3:
		//Assassin
		//unit = new AUnit("assasin", selectedBlock, this);
        type = "assassin";
        strength = 0;
	case 4:
		//Scout
		//unit = new AUnit("scout", selectedBlock, this);
        type = "scout";
        strength = 0;
	default:
        type = "scout";
        strength = 1;
	}
    FVector location = selectedBlock->BlockLocation;
    location[2] += 100.0f; //offset so theres no collison trying to spawn
    unit = GetWorld()->SpawnActor<AUnit>(location, FRotator(0,0,0));
	selectedBlock->setUnit(unit);
    selectedBlock->unit->initializ(type,strength,selectedBlock, this, selectedBlock->row, selectedBlock->column);
    //this->unitList.push_back(unit);
}

void AFinalProjectBlockGrid::moveUnits()
{
    //iterate through blocks and call their movement
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < columns; j++)
        {
            if(grid[i][j]->unit)
                grid[i][j]->unit->move();
        }
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNode(int row, int column)
{
    return this->grid[row][column];
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthNode(AFinalProjectBlock* node)
{
    if(node == NULL)
        return NULL;
    int row = node->row;
    int col = node->column;
    if (row == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row - 1, col);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getSouthNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == this->rows)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getEastNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (col == this->columns)
    {
        return NULL;
    }
    else
    {
        return getNode(row, col + 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getWestNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (col == this->columns)
    {
        return NULL;
    }
    else
    {
        return getNode(row, col - 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthEastNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == this->rows || col == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row - 1, col + 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthWestNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == 0 || col == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row - 1, col - 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getSouthEastNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == this->rows || col == this->columns)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col + 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getSouthWestNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == 0 || col == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col - 1);
    }
}

#undef LOCTEXT_NAMESPACE
