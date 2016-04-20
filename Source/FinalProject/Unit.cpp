// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalProject.h"
#include "Unit.h"
#include "PlayerInfo.h"


// Sets default values
AUnit::AUnit()
{
    UE_LOG(LogTemp, Warning, TEXT("DEBUG: in unit constructor") );
    // Structure to hold one-time initialization
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> WhiteMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlackMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> GreenMaterial;
        FConstructorStatics()
        : PlaneMesh(TEXT("/Game/Puzzle/Meshes/soldier")) //change these meshes and materials for each unit type
        , BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
        , OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
        , WhiteMaterial(TEXT("/Game/Puzzle/Meshes/WhiteMaterial.WhiteMaterial"))
        , BlackMaterial(TEXT("/Game/Puzzle/Meshes/BlackMaterial.BlackMaterial"))
        , RedMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
        , GreenMaterial(TEXT("/Game/Puzzle/Meshes/GreenMaterial.GreenMaterial"))
        //add in other materials for unit diversification
        {
        }
    };
    
    static FConstructorStatics ConstructorStatics;
    // Create dummy root scene component
    DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
    RootComponent = DummyRoot;
    
    // Save a pointer to the orange material
    OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
    BlueMaterial = ConstructorStatics.BlueMaterial.Get();
    WhiteMaterial = ConstructorStatics.WhiteMaterial.Get();
    BlackMaterial = ConstructorStatics.BlackMaterial.Get();
    RedMaterial = ConstructorStatics.RedMaterial.Get();
    GreenMaterial = ConstructorStatics.GreenMaterial.Get();
    // Create static mesh component
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMesh0"));
    mesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
    mesh->SetRelativeScale3D(FVector(3.0f,3.0f,3.0f));
    mesh->SetRelativeLocation(FVector(0.f,0.f,30.f));
    //mesh->setRelativeRotation(90);
    mesh->SetMaterial(0, WhiteMaterial);
    mesh->AttachTo(DummyRoot);

}

void AUnit::initializ(int u, int strength, AFinalProjectBlock* node,
	AFinalProjectBlockGrid* grid, int row, int column, APlayerInfo* player)
{
	switch (u) {
	case 0:
		//King
		type = "king";
		cost = 0;
		mesh->SetMaterial(0, BlueMaterial);
		break;
	case 1:
		//Knight
		type = "knight";
		cost = 3;
		mesh->SetMaterial(0, OrangeMaterial);
		break;
	case 2:
		//Soldier
		type = "soldier";
		cost = 2;
		mesh->SetMaterial(0, WhiteMaterial);
		break;
	case 3:
		//Assassin
		type = "assassin";
		cost = 4;
		mesh->SetMaterial(0, BlackMaterial);
		break;
	case 4:
		//Scout
		type = "scout";
		cost = 1;
		mesh->SetMaterial(0, RedMaterial);
		break;
	case 5:
		//Guard
		type = "guard";
		cost = 5;
		mesh->SetMaterial(0, GreenMaterial);
	default:
		UE_LOG(LogTemp, Warning, TEXT("In Unit: Invalid Unit Type %d"),u);
		return;
	}

    this->strength = strength;
    
    this->location = node->BlockLocation;
    this->rowLocation = row;
    this->columnLocation = column;
    this->grid = grid;
    this->currentNode = node;
    this->movedLeft = false;
    this->hasMoved = false;
    this->soldiersNear = false;
	this->alive = true;
	this->reverseAssassin = false;

	this->owningPlayer = player;
	if (type == "king")
		owningPlayer->king = this;

	//if (this->owningPlayer == currentNode->OwningGrid->player2)
	//	this->movedLeft = true;

	if (type == "assassin")
	{
		//find which side of the board is closest
		if (columnLocation < this->grid->columns / 2)
		{
			//left side of the board
			assassinLeft = false;
		}
		else
		{
			assassinLeft = true;
		}
	}
	

}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	//Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AUnit::player2Move()
{
	AFinalProjectBlock* destination;
	destination = currentNode;
	if (grid->getSouthNode(destination) == NULL) //if the unit has hit the other side of the board do nothing (for now)
	{
		if (type != "assassin")
		{
			//hasMoved = !hasMoved;
			currentNode->OwningGrid->killList.push_back(this);
			owningPlayer->money += (this->cost / 2);
			currentNode->unit = NULL;
			currentNode->clear = true;
			return;
		}
		else
		{
			reverseAssassin = !reverseAssassin;
		}
	}
	if (type == "soldier")
	{
		if (this->soldiersNear)
		{
			UE_LOG(LogTemp, Warning, TEXT("soldier near"));
			destination = grid->getSouthNode(currentNode);
		}
		else
		{
			//TO DO - find closest node with a soldier in it and move one space closer
			destination = getClosestSoldier();
		}
	}
	else if (type == "knight")
	{
		if (movedLeft)
		{
			destination = grid->getSouthWestNode(currentNode);
		}
		else
		{
			destination = grid->getSouthEastNode(currentNode);
		}
		movedLeft = !movedLeft;
		if (destination == NULL)
			this->player2Move(); //if the knight hits the side of the board then go the other way
	}
	else if (type == "scout")
	{
		destination = grid->getSouthNode(grid->getSouthNode(currentNode)); //scout moves 2 spaces forward
		if (destination == NULL)
			return;
	}
	else if (type == "assassin")
	{
		//moves diagonally
		//find which side is closest
		if (assassinLeft)
		{
			//left side of the board
			destination = grid->getSouthEastNode(currentNode);
		}
		else
		{
			destination = grid->getSouthWestNode(currentNode);
		}
		if (destination == NULL)
		{
			assassinLeft = !assassinLeft;
			this->player2Move();
		}

	}
	else //its a guard or king
	{
		destination = currentNode;
	}
	if (destination != NULL)
	{
		if (destination->clear && destination != currentNode)
		{
			destination->unit = this;
			destination->clear = false;
			this->currentNode->clear = true;
			this->currentNode->unit = NULL;
			this->currentNode = destination;
			FVector BlockLocation = destination->GetActorLocation();
			// Move it slightly
			BlockLocation.Z += 100.0f;
			// Set the location- this will blindly place the actor at the given location
			SetActorLocation(BlockLocation, false);
		}
		else
		{
			//conflicts
			this->unitConflict(destination);
		}
	}
}

void AUnit::move()
{

    AFinalProjectBlock* destination;
    destination = currentNode;
    if(grid->getNorthNode(destination) == NULL) //if the unit has hit the other side of the board do nothing (for now)
    {
		if (type != "assassin")
		{
			//hasMoved = !hasMoved;
			currentNode->OwningGrid->killList.push_back(this);
			owningPlayer->money += (this->cost / 2);
			currentNode->unit = NULL;
			currentNode->clear = true;
			return;
		}
		else
		{
			reverseAssassin = !reverseAssassin;
		}
    }
    
    if (type == "soldier")
    {
        if(this->soldiersNear)
        {
            UE_LOG(LogTemp, Warning, TEXT("soldier near"));
            destination = grid->getNorthNode(currentNode);
        }
        else
        {
            //TO DO - find closest node with a soldier in it and move one space closer
            destination = getClosestSoldier();
        }
    }
    else if(type == "knight")
    {
        if(movedLeft)
        {
            destination = grid->getNorthEastNode(currentNode);
        }
        else
        {
            destination = grid->getNorthWestNode(currentNode);
        }
        movedLeft = !movedLeft;
        if(destination == NULL)
            this->move(); //if the knight hits the side of the board then go the other way
    }
    else if(type == "scout")
    {
        destination = grid->getNorthNode(grid->getNorthNode(currentNode)); //scout moves 2 spaces forward
        if(destination == NULL)
            return;
    }
    else if(type == "assassin")
    {
        //moves diagonally
		//find which side of the board is closest
		if (assassinLeft)
		{
			//left side of the board
			destination = grid->getNorthWestNode(currentNode);
		}
		else
		{
			destination = grid->getNorthEastNode(currentNode);
		}
		if (destination == NULL)
		{
			assassinLeft = !assassinLeft;
			this->move();
		}
    }
    else //its a guard or king
    {
        destination = currentNode;
    }
    if(destination != NULL)
    {
        if(destination->clear && destination != currentNode)
        {
            destination->unit = this;
            destination->clear = false;
            this->currentNode->clear = true;
            this->currentNode->unit = NULL;
            this->currentNode = destination;
            //FVector ActorLocation = GetActorLocation();
            FVector BlockLocation = destination->GetActorLocation();
            // Move it slightly
            BlockLocation.Z += 100.0f;
            // Set the location- this will blindly place the actor at the given location
            SetActorLocation( BlockLocation, false );
        }
		else
		{
			this->unitConflict(destination);
		}
    }
}

void AUnit::unitConflict(AFinalProjectBlock* destination)
{
	AUnit* enemy = destination->unit;
	if (this->owningPlayer == destination->unit->owningPlayer) //freindly unit
	{
		return;
	}
	else
	{
		//enemy unit
		if (this->strength > enemy->strength)
		{
			this->currentNode->clear = true;
			this->currentNode->unit = NULL;
			this->currentNode = destination;
			//remove enemy unit from the players list
			//enemy->owningPlayer->unitList.remove(enemy);
			enemy->alive = false;
			//delete it from the game world
			//enemy->Destroy();
			currentNode->OwningGrid->killList.push_back(enemy);
			destination->unit = this;
			FVector BlockLocation = destination->GetActorLocation();
			// Move it slightly
			BlockLocation.Z += 100.0f;
			// Set the location- this will blindly place the actor at the given location
			SetActorLocation(BlockLocation, false);
		}
		else if(this->strength < enemy->strength)
		{
			this->currentNode->clear = true;
			this->currentNode->unit = NULL;
			//this->owningPlayer->unitList.remove(this);
			this->alive = false;
			//this->Destroy();
			currentNode->OwningGrid->killList.push_back(this);
		}
		else
		{
			//kill both the units
			destination->clear = true;
			destination->unit = NULL;
			//enemy->owningPlayer->unitList.remove(enemy);
			enemy->alive = false;
			//enemy->Destroy();
			currentNode->OwningGrid->killList.push_back(enemy);

			this->currentNode->clear = true;
			this->currentNode->unit = NULL;
			//this->owningPlayer->unitList.remove(this);
			this->alive = false;
			//this->Destroy();
			currentNode->OwningGrid->killList.push_back(this);
		}

		if (enemy->type == "knight")
		{
			enemy->strength = enemy->strength - this->strength;
		}
	}

}

AFinalProjectBlock* AUnit::getClosestSoldier()
{
    AFinalProjectBlock* temp = NULL;
    AFinalProjectBlock* result;
    int distance = 0;
    
    for(int i = 0; i < grid->columns; i++)
    {
        AFinalProjectBlock* node = grid->grid[this->rowLocation][i];
        if(node->unit)
        {
            UE_LOG(LogTemp, Warning, TEXT("found unit in row"));
            if(node->unit->type == "soldier" && node->unit != this && node->unit->owningPlayer == this->owningPlayer)
            {
                UE_LOG(LogTemp, Warning, TEXT("DEBUG 1!!!!!!!!!"));
                int tempDis = grid->getDistance(node, this->currentNode);
                if(distance == 0 || distance > tempDis)
                {
                    distance = tempDis;
                    temp = node; //closest soldier
                }
            }
        }
    }
    if(temp != NULL)
    {
        if(temp->column < currentNode->column)
        {
            result = grid->getWestNode(currentNode);
            UE_LOG(LogTemp, Warning, TEXT("west node"));
        }
        else
        {
            result = grid->getEastNode(currentNode);
            UE_LOG(LogTemp, Warning, TEXT("east node"));
        }
    }
    else
        result = NULL;
    
    return result;
}

bool AUnit::checkSoldiers()
{
	this->strength = 0;
	bool result = false;
    AFinalProjectBlock* neighbor;
    neighbor = grid->getNorthNode(currentNode);
    if(neighbor)
    {
        if(neighbor->unit != NULL)
        {
			if (neighbor->unit->type == "soldier" && neighbor->unit->owningPlayer == this->owningPlayer)
			{
				this->strength += 1;
				result = true;
			}
        }
    }
    
    neighbor = grid->getSouthNode(currentNode);
    if(neighbor)
    {
        if(neighbor->unit != NULL)
        {
            if (neighbor->unit->type == "soldier" && neighbor->unit->owningPlayer == this->owningPlayer)
			{
				this->strength += 1;
				result = true;
			}
        }
    }
    
    neighbor = grid->getEastNode(currentNode);
    if(neighbor)
    {
        if(neighbor->unit != NULL)
        {
            if (neighbor->unit->type == "soldier" && neighbor->unit->owningPlayer == this->owningPlayer)
			{
				this->strength += 1;
				result = true;
			}
        }
    }
    
    neighbor = grid->getWestNode(currentNode);
    if(neighbor)
    {
        if(neighbor->unit != NULL)
        {
            if (neighbor->unit->type == "soldier" && neighbor->unit->owningPlayer == this->owningPlayer)
			{
				this->strength += 1;
				result = true;
			}
        }
    }
    
    return result;
}


