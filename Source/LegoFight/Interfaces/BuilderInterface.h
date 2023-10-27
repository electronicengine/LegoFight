// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Camera/CameraComponent.h"
#include "PlugInterface.h"
#include "GameFramework/Character.h"
#include "UObject/Object.h"

#include "BuilderInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuilderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOFIGHT_API IBuilderInterface
{
	GENERATED_BODY()


protected:

	UMaterial* Ghost_Possible_Material;
	UMaterial* Ghost_Imposible_Material;
	bool Building;
	int Tick_Count;
	bool Brick_Plugable;
	int Offset_Step;
	FVector Aim_Impact_Point;
	class ULegoFightGameInstance* Game_Instance;

	IPlugInterface* Target_Plugable_Item;
	bool Brick_Just_Plug;
	int Pivot_Width;
	ABrick* Ghost_Overlapped_Brick;
	AActor* Object_NearBy;

	FRotator OffSet_Rotation;
	FVector OffSet_Location;
	UStaticMeshComponent* Ghost;

public:

	IBuilderInterface();
	void buildStart();
	void buildEnd();
	void grapObject(ABrick* Object);
	void dropObject(ABrick* Object);
	void plugObject();
	void giveOffsetRotation();
	void giveOffsetLocation();
	void lookForBuildingSpace(AActor* Target, FHitResult& OutHit);
	void checkGhostItemOverlap(AActor* OtherActor);

	ABrick* Grabbable_Brick;
	bool Keeping_Bricks;

};
