// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalProject.h"
#include "Unit.h"


// Sets default values
AUnit::AUnit()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AUnit::AUnit(std::string type)
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
        : PlaneMesh(TEXT("/Game/Puzzle/Meshes/Cone.Cone"))
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
    AFinalProjectBlock* node;
    
    if (type == "soldier")
    {
        
    }
    else if(type == "knight")
    {
        
    }
    else if(type == "scout")
    {
        
    }
    else if(type == "assassin")
    {
        
    }
    else if(type == "guard")
    {
        
    }
    else if(type == "king")
    {
        
    }
}

