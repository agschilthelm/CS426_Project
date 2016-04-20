// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "FinalProjectBlockGrid.h"
#include <string>
#include <sstream>
#include "Unit.generated.h"

class AFinalProjectBlockGrid;
class AFinalProjectBlock;
class APlayerInfo;
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
    bool movedLeft; //used for tracking the knights previous movestep
    bool hasMoved; //tracks if the unit has moved this turn
    std::string type;
	bool assassinLeft;
	bool reverseAssassin;
	APlayerInfo* owningPlayer;
    
	bool alive;

    int rowLocation;
    int columnLocation;
    FVector location;
    AFinalProjectBlockGrid* grid;
    AFinalProjectBlock* currentNode;
    
    /** Pointer to materials used on units */
    UPROPERTY()
    class UMaterialInstance* OrangeMaterial;
    UPROPERTY()
    class UMaterialInstance* BlueMaterial;
    UPROPERTY()
    class UMaterialInstance* WhiteMaterial;
    UPROPERTY()
    class UMaterialInstance* BlackMaterial;
    UPROPERTY()
    class UMaterialInstance* RedMaterial;
    UPROPERTY()
    class UMaterialInstance* GreenMaterial;
    
    //executes the units movement based on the rules
    void move();
	void unitConflict(AFinalProjectBlock* destination);
	void player2Move();
    //used for soldiers checks for adjacent soldiers
    bool checkSoldiers();
    //gets node of closest soldier
    AFinalProjectBlock* getClosestSoldier();
    bool soldiersNear;
	// Sets default values for this pawn's properties
	AUnit();
    
    //UFUNCTION()
    void initializ(int u, int strength, AFinalProjectBlock* node,
		AFinalProjectBlockGrid* grid, int row, int column, APlayerInfo* player);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	
	
};
