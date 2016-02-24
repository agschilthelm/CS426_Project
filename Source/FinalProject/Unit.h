// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "FinalProjectBlockGrid.h"
#include <string>
#include <sstream>
#include "Unit.generated.h"

class AFinalProjectBlockGrid;
class AFinalProjectBlock;
UCLASS()
class FINALPROJECT_API AUnit : public APawn
{
	GENERATED_BODY()
    /** Dummy root component */
    UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USceneComponent* DummyRoot;
    
    /** StaticMesh component for the units appearance */
    UPROPERTY()
    class UStaticMeshComponent* mesh;
    
public:
    int strength;
    int cost;
    std::string type;
    
    int rowLocation;
    int columnLocation;
    AFinalProjectBlockGrid* grid;
    
    //executes the units movement based on the rules
    void move();
    
	// Sets default values for this pawn's properties
	AUnit();
    AUnit(std::string type);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
