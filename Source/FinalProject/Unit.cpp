// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalProject.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AUnit::AUnit(std::string type, AFinalProjectBlock* node)
{
    
    this->type = type;
    this->strength = 1;
    this->cost = 1;
    
    // Structure to hold one-time initialization
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
        ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
        FConstructorStatics()
        : PlaneMesh(TEXT("/Game/Puzzle/Meshes/Cone.Cone")) //change these meshes and materials for each unit type
        , BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
        , OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
        {
        }
    };
    
    static FConstructorStatics ConstructorStatics;
    // Create dummy root scene component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
    RootComponent = DummyRoot;
    
    // Create static mesh component
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMesh0"));
    mesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
    mesh->SetRelativeScale3D(FVector(1.f,1.f,0.02f));
    mesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
    mesh->SetMaterial(0, ConstructorStatics.OrangeMaterial.Get());
    mesh->AttachTo(DummyRoot);
    
    this->grid = NULL;
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

