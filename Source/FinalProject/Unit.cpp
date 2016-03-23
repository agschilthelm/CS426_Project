// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalProject.h"
#include "Unit.h"


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
        : PlaneMesh(TEXT("/Game/Puzzle/Meshes/Cone.Cone")) //change these meshes and materials for each unit type
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
    mesh->SetRelativeScale3D(FVector(1.5f,1.5f,1.5f));
    mesh->SetRelativeLocation(FVector(0.f,0.f,30.f));
    mesh->SetMaterial(0, WhiteMaterial);
    mesh->AttachTo(DummyRoot);

}

void AUnit::initializ(int u, int strength, AFinalProjectBlock* node, AFinalProjectBlockGrid* grid, int row, int column)
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
		cost = 5;
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

}

// Called when the game starts or when spawned
void AUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnit::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AUnit::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AUnit::move()
{
    AFinalProjectBlock* destination;
    destination = grid->getNorthNode(currentNode);
    if(destination == NULL) //if the unit has hit the other side of the board do nothing (for now)
        return;
    
    if (type == "soldier")
    {
        if(checkSoldiers())
            destination = grid->getNorthNode(currentNode);
        else
        {
            //TO DO - find closest node with a soldier in it and move one space closer
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
    }
    else if(type == "assassin")
    {
        //finds shortest path to enemy king when spawned and moves one space along it
    }
    else //its a guard or king
    {
        destination = currentNode;
    }
    if(destination->clear)
    {
        destination->unit = this;
        destination->clear = false;
        this->currentNode = destination;
        //TO DO - translate the unit in the game
    }
    else
    {
        //TO DO - deal with conflicts if node is occupied by friendly or enemy
        
    }
}

bool AUnit::checkSoldiers()
{
    AFinalProjectBlock* neighbor;
    neighbor = grid->getNorthNode(currentNode);
    if(neighbor && neighbor->unit->type == "soldier")
        return true;
    
    neighbor = grid->getSouthNode(currentNode);
    if(neighbor && neighbor->unit->type == "soldier")
        return true;
    
    neighbor = grid->getEastNode(currentNode);
    if(neighbor && neighbor->unit->type == "soldier")
        return true;
    
    neighbor = grid->getWestNode(currentNode);
    if(neighbor && neighbor->unit->type == "soldier")
        return true;
    
    return false;
}

