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

void AFinalProjectBlockGrid::moveUnits() {
	UE_LOG(LogTemp, Warning, TEXT("MOVING UNITS!!!\n"));
	//iterate through blocks and call their movement
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (grid[i][j]->unit)
				grid[i][j]->unit->move();
		}
	}
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

void AFinalProjectBlockGrid::SetActive(int32 r, int32 c)
{
    if(selectedBlock != NULL)
        selectedBlock->Unselect();
	selectedBlock = getNode(r,c);
}

void AFinalProjectBlockGrid::setUnit(int32 u){
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("setUnit"));
    
	if (selectedBlock == NULL || selectedBlock->clear == false) {
		return;
	}

	bool legalplacement = false;
	if(u != (int32)0){
		UE_LOG(LogTemp, Warning, TEXT("NOT A KING"));
		for(int i = -2; i<3;i++){
			for(int j = -2;j<3;j++){
				if(selectedBlock->row + i >= 0 && selectedBlock->column + j >= 0 && !legalplacement){
					legalplacement = !(getNode(selectedBlock->row + i, selectedBlock->column + j)->clear);
				}
			}
		}
		if (!legalplacement){
			UE_LOG(LogTemp, Warning, TEXT("INVALID PLACEMENT LOCATION, NO UNIT W/IN 2\n"));
			return;
		}
	}
	

	AUnit *unit;
    int strength;
	if (u == 3)
		UE_LOG(LogTemp, Warning, TEXT("ASSASSIN CREATING!!!\n"));

	switch ((int)u) {
	case 0:
		//King
        strength = 0;
		break;
	case 1:
		//Knight
        strength = 0;
		break;
	case 2:
		//Soldier
        strength = 0;
		break;
	case 3:
		//Assassin
        strength = 0;
		break;
	case 4:
		//Scout
        strength = 0;
		break;
	case 5:
		//Guard
		strength = 5;
		break;
	default:
        UE_LOG(LogTemp, Warning, TEXT("in Grid: INVALID UNIT TYPE %d\n"), u);
		return;
	}
    FVector location = selectedBlock->BlockLocation;
    location[2] += 100.0f;
    //UE_LOG(LogTemp, Warning, TEXT("DEBUG: block location: %f"), selectedBlock->BlockLocation[2]);
    //UE_LOG(LogTemp, Warning, TEXT("DEBUG: unit location: %f"), location[2]);
    unit = GetWorld()->SpawnActor<AUnit>(location, FRotator(0,0,0));
	selectedBlock->setUnit(unit);
    selectedBlock->unit->initializ(u,strength,selectedBlock, this, selectedBlock->row, selectedBlock->column);
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
