// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <list>
#include "GameFramework/Actor.h"
#include "PlayerInfo.generated.h"


class AUnit;
UCLASS()
class FINALPROJECT_API APlayerInfo : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* playerMesh;
	
public:	

	std::list<AUnit*> unitList;

	UPROPERTY(Category = "player",EditAnywhere, BlueprintReadWrite)
	int32 money;

	void addUnit(AUnit* unit);
	int turnCount;
	bool kingAlive;
	AUnit* king;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
		class UMaterialInstance* OrangeMaterial;

	/** Pointer to Blue material used on active blocks */
	UPROPERTY()
		class UMaterialInstance* BlueMaterial;

	// Sets default values for this actor's properties
	APlayerInfo();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
