// Fill out your copyright notice in the Description page of Project Settings.


#include "BuiltInInterface.h"
#include "../Vehicles/EnemyLegoVehicle.h"
#include "../Brick.h"
#include "../LegoFightGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Widgets/InventoryWidget.h"


// Add default functionality here for any IBuiltInInterface functions that are not pure virtual.


ConstructionInfo IBuiltInInterface::compileConstructInfo(IBuiltInInterface *Object)
{
    ConstructionInfo build_info;


    ALegoCarChasis* vehicle = Cast<ALegoCarChasis>(Object);

    if (vehicle)
    {

        if (Cast<AEnemyLegoVehicle>(vehicle)) {
            build_info.Item_Name = "vehicle_ai";
        }
        else {
            build_info.Item_Name = "vehicle";
        }
        build_info.Item_Color = Colors["White"];
        build_info.Plugin_Index = 0;
        build_info.Offset_Rotation = vehicle->Offset_Rotation;
        build_info.Offset_Location = vehicle->Offset_Location;


        for (std::pair<int, ABrick*> var : vehicle->Plugged_Items_OnIt)
        {
            ConstructionInfo brick_info;
            brick_info = compileConstructInfo(var.second);

            build_info.Plugged_Items.push_back(brick_info);
        }

    }
    else
    {
        ABrick* brick = Cast<ABrick>(Object);
        if (brick)
        {

            build_info.Item_Name = brick->Brick_Name;
            build_info.Item_Color = brick->Brick_Color;

            build_info.Plugin_Index = brick->Own_Plugin_Index;
            build_info.Offset_Rotation = brick->Offset_Rotation;
            build_info.Offset_Location = brick->Offset_Location;


            for (std::pair<int, ABrick *> var : brick->Plugged_Items_OnIt)
            {
                ConstructionInfo brick_info;
                brick_info = compileConstructInfo(var.second);

                build_info.Plugged_Items.push_back(brick_info);
            }

        }

    }
    
   

    return build_info;
}



void IBuiltInInterface::buildFromConstructionInfo(const ConstructionInfo &Info, const FVector &BuildLocation,
                                                  ULegoFightGameInstance *GameInstance, ABrick *MainBrick)
{

    ALegoCarChasis *vehicle = nullptr;
    ABrick *main_brick = nullptr;
    AWeapon* weapon = nullptr;
    AConstraitBrick* machine = nullptr;

    if(Info.Item_Name.Find(VEHICLE_APPENDIX) >= 0)
    {
        vehicle = GameInstance->spawnVehicle(Info.Item_Name, BuildLocation + FVector(0,0,10.0f), FRotator(0,0,0));
    }
    else
    {
        if(MainBrick == nullptr)
        {
            GameInstance->selectCurrentProductColor(Info.Item_Color);
            if (Info.Item_Name.Find(MACHINE_APPENDIX) >= 0) {
                main_brick = Cast<ABrick>(GameInstance->spawnMachine(Info.Item_Name, FVector(0, 0, 0), FRotator(0, 0, 0)));
                main_brick->putTheItem(BuildLocation + FVector(0, 0, 10.0f), FRotator(0, 0, 0));
            }
            else {
                main_brick = GameInstance->spawnBrick(Info.Item_Name, FVector(0, 0, 0), FRotator(0, 0, 0));
                main_brick->putTheItem(BuildLocation + FVector(0, 0, 10.0f), FRotator(0, 0, 0));
            }

        }
        else
        {
            main_brick = MainBrick;
        }
    }



    for(int i=0; i<Info.Plugged_Items.size(); i++)
    {
        ABrick* brick;

        GameInstance->selectCurrentProductColor(Info.Plugged_Items[i].Item_Color);
        if (Info.Plugged_Items[i].Item_Name.Find(WEAPON_APPENDIX) > 0) {
            brick = Cast<ABrick>(GameInstance->spawnWeapon(Info.Plugged_Items[i].Item_Name, FVector(0, 0, 0), FRotator(0, 0, 0)));
        }
        else if(Info.Plugged_Items[i].Item_Name.Find(MACHINE_APPENDIX) > 0) {
            FVector location;
            if(main_brick)
                location = main_brick->GetActorLocation() + FVector(50, 0, 0);
            else if(vehicle)
                location = vehicle->GetActorLocation() + FVector(50, 0, 0);

            brick = Cast<ABrick>(GameInstance->spawnMachine(Info.Plugged_Items[i].Item_Name, location, FRotator(0, 0, 0)));    
            brick->enablePhysics(false);
            brick->setCollisionProfile("OverlapAll");
        }
        else{
            brick = GameInstance->spawnBrick(Info.Plugged_Items[i].Item_Name, FVector(0, 0, 0), FRotator(0, 0, 0));
        }

        if(vehicle != nullptr)
        {
            vehicle->plugTheItem(brick, Info.Plugged_Items[i].Plugin_Index,
                Info.Plugged_Items[i].Offset_Rotation, 
                Info.Plugged_Items[i].Offset_Location);
        }
        else if(main_brick != nullptr)
        {
            main_brick->plugTheItem(brick, Info.Plugged_Items[i].Plugin_Index, 
                Info.Plugged_Items[i].Offset_Rotation, 
                Info.Plugged_Items[i].Offset_Location);
        }


        buildFromConstructionInfo(Info.Plugged_Items[i], BuildLocation, GameInstance, brick);

    }
}
