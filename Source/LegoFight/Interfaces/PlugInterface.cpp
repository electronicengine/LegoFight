// Fill out your copyright notice in the Description page of Project Settings.


#include "PlugInterface.h"
#include "../Brick.h"
#include "../Vehicles/LegoCarChasis.h"
#include <vector>
#include "Engine/Engine.h"
#include "UObject/Object.h"
#include "Kismet/KismetSystemLibrary.h"


// Add default functionality here for any IPlugInterface functions that are not pure virtual.
void IPlugInterface::setupPluginPoints(const FVector &Begining_Pivot, int PluginNumberWidth, int PluginNumberHeight)
{

    FString scene_name("scene");
    USceneComponent *scene;
    FVector offset = FVector(0,0,0);

    int counter = 0;

    int plugin_number = PluginNumberWidth * PluginNumberHeight;

    for(int i=0; i < plugin_number; i++)
    {

        scene_name = "Scene";
        scene_name.AppendInt(i);

        scene = CreatePluginPoint(scene_name);

        Plugin_Points.Push(scene);
    }

    // place Plugin Pivots
    for(int i=0; i<PluginNumberHeight; i++)
    {
        for(int k = 0; k<PluginNumberWidth; k++)
        {
            Plugin_Points[counter++]->SetRelativeLocation(Begining_Pivot + offset);
            offset.Y += (BRICK_LENGHT / 2);
        }

        offset.X -= (BRICK_LENGHT / 2);
        offset.Y = 0;
    }

}


void IPlugInterface::setupSidePlugPoints(const FVector& Begining_Pivot, int PluginNumberWidth, int PluginNumberHeight)
{

    FString scene_name("sidescene");
    USceneComponent* scene;
    FVector offset = FVector(0, 0, 0);

    int counter = Plugin_Points.Num();

    int plugin_number = PluginNumberWidth * PluginNumberHeight;

    for (int i = 0; i < plugin_number; i++)
    {

        scene_name = "sidescene";
        scene_name.AppendInt(i);

        scene = CreatePluginPoint(scene_name);
        scene->SetRelativeRotation(FRotator(0, 0, 90));

        Plugin_Points.Push(scene);
    }

    scene = CreatePluginPoint("last");


    // place Plugin Pivots
    for (int i = 0; i < PluginNumberHeight; i++)
    {
        for (int k = 0; k < PluginNumberWidth; k++)
        {
            Plugin_Points[counter++]->SetRelativeLocation(Begining_Pivot + offset);
            offset.Z += (BRICK_LENGHT / 2);
        }

        offset.X -= (BRICK_LENGHT / 2);
        offset.Z = 0;
    }
}


float IPlugInterface::calculateDistance(const FVector &Vector1, const FVector &Vector2)
{
    FVector delta = Vector1 - Vector2;

    return delta.Size();

    return 0.f;
}



bool IPlugInterface::highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
                             ABrick *Interactable_Brick, const FHitResult &OutHit, const FRotator &OffetRotation, const FVector& OffetLocation, ABrick *OverlappedBrick)
{

    FVector plugin = getPlugin(OutHit.ImpactPoint);
    FRotator plugin_rotation = getPluginRotation();

    Ghost_Brick->SetStaticMesh(Interactable_Brick->getBrickMesh());
    Ghost_Brick->SetWorldScale3D(FVector(0.85f, 0.85f, 0.85f));


    if(plugin != FVector(0,0,0))
    {


        Ghost_Brick->SetVisibility(true);

        Ghost_Brick->SetWorldLocationAndRotation(plugin,
                                                     plugin_rotation);

        Ghost_Brick->AddLocalRotation(OffetRotation);

        Ghost_Brick->AddLocalOffset(FVector(0, 0, (Interactable_Brick->Height_Offset) * -1));
        Ghost_Brick->AddLocalOffset(OffetLocation);


        if(Ghost_Brick->IsOverlappingActor(OverlappedBrick))
        {
            Ghost_Brick->SetMaterial(0, ImPossible_Material);
            return true;
        }
        else
        {
            Ghost_Brick->SetMaterial(0, Possible_Material);
            return true;
        }
    }
    else
    {

        Ghost_Brick->SetVisibility(false);
        return false;
    }

    return true;
}



FVector IPlugInterface::getPlugin(const FVector &ImpactPoint)
{

    if(Plugin_Points.Num() == 0)
        return FVector(0,0,0);

    float distance;
    std::vector<float> distance_array;


    for(int i=0; i<Plugin_Points.Num(); i++)
    {
        distance = calculateDistance(ImpactPoint, (Plugin_Points[i]->GetComponentLocation()));

        distance_array.push_back(distance);
    }

    Current_Plugin_Index = getClosestPluginIndex(distance_array);

    return Plugin_Points[Current_Plugin_Index]->GetComponentLocation();

}



int IPlugInterface::getClosestPluginIndex(const std::vector<float> &Array)
{

    float dist = 4000000;
    int index = 0;

    for(int i=0; i<Array.size(); i++)
    {
        if(Array[i] < dist)
        {
            index = i;
            dist = Array[i];
        }
    }

    return index;

}


void IPlugInterface::plugTheBrick(ABrick *Object, int PluginIndex, const FRotator &OffsetRotation, const FVector &OffsetLocation)
{

    FVector plug_location;
    FRotator plug_rotation;

    if(PluginIndex < 0)
        PluginIndex = Current_Plugin_Index;


    if(PluginIndex < Plugin_Points.Num())
    {
        plug_location = Plugin_Points[PluginIndex]->GetComponentLocation();
        plug_rotation = Plugin_Points[PluginIndex]->GetComponentRotation();
        
        //if(Object->Type_ == CarSeat2x2)
        //{
        //    if(Owner_Car != nullptr)
        //        Owner_Car->addSeatToCar(Cast<ACarSeat>(Object));
        //    else if(Cast<ALegoCarChasis>(this))
        //        Cast<ALegoCarChasis>(this)->addSeatToCar(Cast<ACarSeat>(Object));
        //}


        if(Cast<AWeapon>(Object))
        {
            if(Owner_Car != nullptr)
                Owner_Car->addWeaponToInventory(Cast<AWeapon>(Object));
            else if(Cast<ALegoCarChasis>(this))
                Cast<ALegoCarChasis>(this)->addWeaponToInventory(Cast<AWeapon>(Object));
        }

        if(Owner_Car != nullptr)
            Object->Owner_Car = Owner_Car;
        else if(Cast<ALegoCarChasis>(this))
            Object->Owner_Car = Cast<ALegoCarChasis>(this);

        const FDetachmentTransformRules &attachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
                                                               EDetachmentRule::KeepWorld,
                                                               EDetachmentRule::KeepWorld, false);
        Object->DetachFromActor(attachment_rules);

        Object->SetActorEnableCollision(true);

        Object->SetActorLocationAndRotation(plug_location, plug_rotation,
                                            false, 0, ETeleportType::None);

        Object->AddActorLocalRotation(OffsetRotation);
        Object->AddActorLocalOffset(OffsetLocation);

        Object->AddActorLocalOffset(FVector(0, 0, (Object->Height_Offset) * -1));


        Object->enablePhysics(true);
        Object->setCollisionProfile("BlockAll");

        Object->AttachToActor(Cast<AActor>(this), FAttachmentTransformRules(EAttachmentRule::KeepWorld,
                                                               EAttachmentRule::KeepWorld,
                                                               EAttachmentRule::KeepWorld, true));

        Plugged_Items_OnIt.push_back(Object);
        Object->Own_Plugin_Index = PluginIndex;
        Object->Offset_Rotation = OffsetRotation;
        Object->Offset_Location = OffsetLocation;

    }
}



void IPlugInterface::autoPlugin(AActor *BelowBrick)
{

    if(Cast<ABrick>(BelowBrick))
    {
        ABrick *below_brick = Cast<ABrick>(BelowBrick);
        FVector plugin = below_brick->getPlugin(Cast<ABrick>(this)->GetActorLocation());
        FRotator plugin_rotation = Cast<ABrick>(this)->GetActorRotation();

    //    SetActorLocationAndRotation(plugin, plugin_rotation);

        below_brick->plugTheBrick(Cast<ABrick>(this), -1, plugin_rotation, FVector(0, 0, 0));
    }
    else if(Cast<ALegoCarChasis>(BelowBrick))
    {
        ALegoCarChasis *below_vehicle = Cast<ALegoCarChasis>(BelowBrick);
        FVector plugin = below_vehicle->getPlugin(Cast<ABrick>(this)->GetActorLocation());
        FRotator plugin_rotation = Cast<ABrick>(this)->GetActorRotation();


    //    SetActorLocationAndRotation(plugin, plugin_rotation);

        below_vehicle->plugTheBrick(Cast<ABrick>(this), -1, plugin_rotation, FVector(0,0,0));
    }
}



FRotator IPlugInterface::getPluginRotation()
{
    if(Plugin_Points.Num()!= 0)
        return Plugin_Points[Current_Plugin_Index]->GetComponentRotation();
    else
        return FRotator(0,0,0);
}

bool IPlugInterface::checkPluginPointAvailable(FVector& Point)
{



    for (int i = 0; i < Plugin_Points.Num(); i++) {

        int abs = (Point - Plugin_Points[i]->GetComponentLocation()).Size();
        if ( abs == 0) {

            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue,
                FString::FromInt((Point - Plugin_Points[i]->GetComponentLocation()).Size()));


            return true;
        }
    }

    return false;
}



void IPlugInterface::releaseAll()
{
    if (this) {
        for (int i = 0; i < Plugged_Items_OnIt.size(); i++) {
            if (Plugged_Items_OnIt[i])
                Plugged_Items_OnIt[i]->releaseAll();
        }

        Cast<ABrick>(this)->enablePhysics(true);

        const FDetachmentTransformRules& detachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
            EDetachmentRule::KeepWorld,
            EDetachmentRule::KeepWorld, true);
        Cast<AActor>(this)->DetachFromActor(detachment_rules);
    }
}


