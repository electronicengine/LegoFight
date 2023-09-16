// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/Object.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include <vector>
#include <map>
#include "PlugInterface.generated.h"

#define BRICK_LENGHT        50

class ABrick;
class ALegoCarChasis;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlugInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOFIGHT_API IPlugInterface
{
	GENERATED_BODY()




	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:


        TArray<USceneComponent *> Plugin_Points;

        std::vector<ABrick *> Plugged_Bricks_OnIt;

        bool plugged;
        int Current_Plugin_Index;
        float Height_Offset;
        int Own_Plugin_Index;
        uint8 Plugin_Number_Width;
        uint8 Plugin_Number_Height;
        uint8 Plugin_Number;
        ALegoCarChasis *Owner_Car;
        FRotator Offset_Rotation;


        virtual USceneComponent *CreatePluginPoint(FString Name) = 0;
        void setupPluginPoints(const FVector &Begining_Pivot, int PluginNumberWidth, int PluginNumberHeight);
        void setupSidePlugPoints(const FVector& Begining_Pivot, int PluginNumberWidth, int PluginNumberHeight);

        float calculateDistance(const FVector &Vector1, const FVector &Vector2);
        int getClosestPluginIndex(const std::vector<float> &Array);
        FVector getPlugin(const FVector &ImpactPoint);
        void plugTheBrick(ABrick *Object, int PluginIndex, const FRotator &OffsetRotation);


        bool highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
                                     ABrick *Interactable_Brick, const FHitResult &OutHit, const FRotator &OffetRotation,ABrick *OverlappedBrick);
        FRotator getPluginRotation();
        void autoPlugin(AActor *TargetBrick);

        void releaseAll();

};
