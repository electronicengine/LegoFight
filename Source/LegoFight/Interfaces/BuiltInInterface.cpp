// Fill out your copyright notice in the Description page of Project Settings.


#include "BuiltInInterface.h"
#include "../Vehicles/EnemyLegoVehicle.h"
#include "../Brick.h"
#include "../LegoFightGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Add default functionality here for any IBuiltInInterface functions that are not pure virtual.


ConstructionInfo IBuiltInInterface::compileConstructInfo(AActor *Object)
{

   AEnemyLegoVehicle *enemy_vehicle = Cast<AEnemyLegoVehicle>(Object);

   ConstructionInfo build_info;

    if(enemy_vehicle)
    {

        build_info.Type_ = Lego_Enemy_Vehicle;
        build_info.Sub_Type = Comp;
        build_info.Plugin_Index = 0;
        build_info.Offset_Rotation = enemy_vehicle->Offset_Rotation;

        for(int i=0; i<enemy_vehicle->Plugged_Bricks_OnIt.size(); i++)
        {
            ConstructionInfo brick_info;
            brick_info = compileConstructInfo(enemy_vehicle->Plugged_Bricks_OnIt[i]);

            build_info.Plugged_Bricks_OnIt.push_back(brick_info);
        }

    }else
    {
        ABrick *brick = Cast<ABrick>(Object);
        if(brick)
        {

            build_info.Type_ = brick->Type_;
            build_info.Sub_Type = brick->Sub_Type;
            build_info.Plugin_Index = brick->Own_Plugin_Index;
            build_info.Offset_Rotation = brick->Offset_Rotation;

            for(int i=0; i<brick->Plugged_Bricks_OnIt.size(); i++)
            {
                ConstructionInfo brick_info;
                brick_info = compileConstructInfo(brick->Plugged_Bricks_OnIt[i]);

                build_info.Plugged_Bricks_OnIt.push_back(brick_info);
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

    if(Info.Type_ >= 30)
    {
        vehicle = GameInstance->spawnVehicle(Info.Type_, BuildLocation + FVector(0,0,10.0f), FRotator(0,0,0));
    }
    else
    {
        if(MainBrick == nullptr)
        {
            main_brick = GameInstance->spawnBrick(Info.Type_, FVector(0,0,0), FRotator(0,0,0));
        }
        else
        {
            main_brick = MainBrick;
        }
    }



    for(int i=0; i<Info.Plugged_Bricks_OnIt.size(); i++)
    {

        ABrick *brick = GameInstance->spawnBrick(Info.Plugged_Bricks_OnIt[i].Type_, FVector(0,0,0), FRotator(0,0,0));

        if(vehicle != nullptr)
        {
            vehicle->plugTheBrick(brick, Info.Plugged_Bricks_OnIt[i].Plugin_Index, Info.Plugged_Bricks_OnIt[i].Offset_Rotation);
        }
        else if(main_brick != nullptr)
        {
            main_brick->plugTheBrick(brick, Info.Plugged_Bricks_OnIt[i].Plugin_Index, Info.Plugged_Bricks_OnIt[i].Offset_Rotation);
        }


        buildFromConstructionInfo(Info.Plugged_Bricks_OnIt[i], BuildLocation, GameInstance, brick);

    }
}
