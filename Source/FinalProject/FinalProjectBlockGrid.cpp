// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FinalProject.h"
#include "FinalProjectBlockGrid.h"
#include "PlayerInfo.h"
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
	endTurn();
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
	this->selectedBlock = grid[0][0];

	player1 = GetWorld()->SpawnActor<APlayerInfo>(FVector(0.f, -100.f, 0.f), FRotator(0, 0, 0));
	player2 = GetWorld()->SpawnActor<APlayerInfo>(FVector(0.f, -200.f, 0.f), FRotator(0, 0, 0));
	activePlayer = player1;
	playerInt = 0;

}

void AFinalProjectBlockGrid::SetActive(int32 r, int32 c)
{
    if(selectedBlock != NULL)
        selectedBlock->Unselect();
	selectedBlock = getNode(r,c);
}

bool AFinalProjectBlockGrid::legalPlace()
{
	bool result = false;
	bool within2 = false;

	//if the unit is a king it must be placed on friendly half of the board

	if (!selectedBlock->clear) //if space is occupied 
		return false;
	else
	{
		//block is clear
		//check to see if within two blocks of a friendly unit
		int furthestRow;
		if (activePlayer == player1)
			furthestRow = 0;
		else
			furthestRow = this->rows;

		std::list<AUnit*>::iterator iter;
		for (iter = activePlayer->unitList.begin(); iter != activePlayer->unitList.end(); iter++)
		{
			if ((*iter) != NULL && (*iter)->alive)
			{
				int distance = getDistance(selectedBlock, (*iter)->currentNode);
				if (distance <= 2)
				{
					within2 = true;
				}
				if (activePlayer == player1)
				{
					if ((*iter)->currentNode->row > furthestRow)
					{
						furthestRow = (*iter)->currentNode->row;
					}
				}
				else
				{
					if ((*iter)->currentNode->row < furthestRow)
					{
						furthestRow = (*iter)->currentNode->row;
					}
				}
			}
		}
		if ((activePlayer == player1 && (selectedBlock->row > furthestRow || !within2)) || (activePlayer == player2 && (selectedBlock->row < furthestRow || !within2)))
			return false;
		else
			result = true;
	}
	return result;
}

void AFinalProjectBlockGrid::setUnit(int32 u){
    
	if (selectedBlock == NULL || selectedBlock->clear == false) {
		return;
	}
	if (activePlayer->turnCount == 0 && !activePlayer->kingAlive)
	{
		u = 0;
		activePlayer->kingAlive = true;
	}
	bool legalplacement = false;
	if(u != (int32)0){
		UE_LOG(LogTemp, Warning, TEXT("NOT A KING"));
		if (!legalPlace()){
			UE_LOG(LogTemp, Warning, TEXT("INVALID PLACEMENT LOCATION\n"));
			return;
		}
	}
	
	AUnit *unit;
    int strength;
	int cost = 0;
	if (u == 3)
		UE_LOG(LogTemp, Warning, TEXT("ASSASSIN CREATING!!!\n"));

	switch ((int)u) {
	case 0:
		//King
		cost = 0;
        strength = 0;
		break;
	case 1:
		//Knight
		cost = 3;
        strength = 2;
		break;
	case 2:
		//Soldier
		cost = 2;
        strength = 1;
		break;
	case 3:
		//Assassin
		cost = 3;
        strength = 3;
		break;
	case 4:
		//Scout
		cost = 1;
        strength = 1;
		break;
	case 5:
		//Guard
		cost = 5;
		strength = 5;
		break;
	default:
        UE_LOG(LogTemp, Warning, TEXT("in Grid: INVALID UNIT TYPE %d\n"), u);
		return;
	}
	activePlayer->money = activePlayer->money - cost;
	if (activePlayer->money < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("player out of money"));
		return;
	}
    FVector location = selectedBlock->BlockLocation;
    location[2] += 100.0f;
    unit = GetWorld()->SpawnActor<AUnit>(location, FRotator(0,0,0));
	selectedBlock->setUnit(unit);
    selectedBlock->unit->initializ(u,strength,selectedBlock,
		this, selectedBlock->row, selectedBlock->column, activePlayer);
	activePlayer->addUnit(unit);
}

void AFinalProjectBlockGrid::endTurn()
{
	activePlayer->turnCount++;
	activePlayer->money = 10;
	if (activePlayer == player1)
	{
		playerInt = 1;
		activePlayer = player2;
		UE_LOG(LogTemp, Warning, TEXT("Switch to player 2\n"));
	}
	else
	{
		this->killList.clear();

		UE_LOG(LogTemp, Warning, TEXT("MOVING UNITS!!!\n"));
		//iterate through units and call their movement
		std::list<AUnit*>::iterator iter;
		for (iter = player1->unitList.begin(); iter != player1->unitList.end(); iter++)
			if (*iter != NULL)
				if ((*iter)->type == "soldier" && (*iter)->alive)
					(*iter)->soldiersNear = (*iter)->checkSoldiers();

		for (iter = player1->unitList.begin(); iter != player1->unitList.end(); iter++)
			if ((*iter) != NULL)
				if((*iter)->alive)
					(*iter)->move();

		for (iter = this->killList.begin(); iter != this->killList.end(); iter++)
			if (*iter != NULL)
			{
				(*iter)->owningPlayer->unitList.remove((*iter));
				(*iter)->Destroy();
			}

		this->killList.clear();

		for (iter = player2->unitList.begin(); iter != player2->unitList.end(); iter++)
			if (*iter != NULL)
				if ((*iter)->type == "soldier" && (*iter)->alive)
					(*iter)->soldiersNear = (*iter)->checkSoldiers();

		for (iter = player2->unitList.begin(); iter != player2->unitList.end(); iter++)
			if (*iter != NULL)
				if ((*iter)->alive)
					(*iter)->player2Move();
		

		for (iter = this->killList.begin(); iter != this->killList.end(); iter++)
			if (*iter != NULL)
			{
				(*iter)->owningPlayer->unitList.remove((*iter));
				(*iter)->Destroy();
			}

		activePlayer = player1;
		playerInt = 0;
	}
		



}

void AFinalProjectBlockGrid::enableAI() {

}

AFinalProjectBlock* AFinalProjectBlockGrid::getNode(int row, int column)
{
    return this->grid[row][column];
}

int AFinalProjectBlockGrid::getDistance(AFinalProjectBlock* node1, AFinalProjectBlock* node2)
{
    int row1 = node1->row;
    int col1 = node1->column;
    int row2 = node2->row;
    int col2 = node2->column;
    return (abs(row2 - row1) + abs(col2 - col1));
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == this->rows - 1)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getSouthNode(AFinalProjectBlock* node)
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

AFinalProjectBlock* AFinalProjectBlockGrid::getEastNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (col == this->columns - 1)
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
    if (col == 0)
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
    if (row == this->rows -1 || col == this->columns -1)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col + 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getNorthWestNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == this->rows -1 || col == 0)
    {
        return NULL;
    }
    else
    {
        return getNode(row + 1, col - 1);
    }
}

AFinalProjectBlock* AFinalProjectBlockGrid::getSouthEastNode(AFinalProjectBlock* node)
{
    int row = node->row;
    int col = node->column;
    if (row == 0 || this->columns -1)
    {
        return NULL;
    }
    else
    {
        return getNode(row - 1, col + 1);
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
        return getNode(row - 1, col - 1);
    }
}

#undef LOCTEXT_NAMESPACE
