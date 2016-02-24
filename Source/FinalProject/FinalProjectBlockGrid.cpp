// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectBlockGrid.h"
#include "Components/TextRenderComponent.h"

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
	BlockSpacing = 275.f;
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

	// Loop to spawn each block
	/*for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		AFinalProjectBlock* NewBlock = GetWorld()->SpawnActor<AFinalProjectBlock>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if(NewBlock != NULL)
		{
			NewBlock->OwningGrid = this;
		}
	}*/
}


void AFinalProjectBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNode(int row, int column)
{
    return this->grid[row][column];
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthNode(AFinalProjectBlock* node)
{
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
    if (row == 0)
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
    if (row == 0)
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
    if (row == 0)
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
    if (row == 0)
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
    if (row == 0)
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
    if (row == 0)
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
    if (row == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col - 1);
    }
}

#undef LOCTEXT_NAMESPACE
