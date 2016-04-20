// Fill out your copyright notice in the Description page of Project Settings.

#include "FinalProject.h"
#include "PlayerInfo.h"


// Sets default values
APlayerInfo::APlayerInfo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	float locationZ = 25.f;
	float sizeZ = 0.01f;
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	playerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	playerMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	playerMesh->SetRelativeScale3D(FVector(1.f, 1.f, sizeZ));
	playerMesh->SetRelativeLocation(FVector(0.f, 0.f, locationZ));
	playerMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	playerMesh->AttachTo(DummyRoot);

	// Save a pointer to the orange material
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();

	this->money = 10;
	this->turnCount = 0;
	kingAlive = false;
	king = NULL;
	UE_LOG(LogTemp, Warning, TEXT("CREATING PLAYER"));
}

void APlayerInfo::addUnit(AUnit* unit)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding unit to player"));
	if(money >= 0)
		this->unitList.push_back(unit);
}

// Called when the game starts or when spawned
void APlayerInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerInfo::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

