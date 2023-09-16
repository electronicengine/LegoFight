// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego1x1Comp.h"
#include <vector>
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"



ALego1x1Comp::ALego1x1Comp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //PrimaryActorTick.bCanEverTick = false;


   /*
   Destructible_Container = ADestrictable::StaticClass();

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_1x1_all.lego_1x1_all'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    Material = material_asset.Object;
    Brick->SetMaterial(0, Material);

    Collision_Box->SetRelativeLocation(FVector(0,0,-5));
    Collision_Box->SetRelativeScale3D(FVector(0.77f, 0.77f, 0.448889f));

    //setupPluginPoints(FVector(25, -25.0f, 30.7f), 3, 3);

    Type_ = Lego1x1_Comp;
    Sub_Type = Comp;

    Current_Plugin_Index = 0;
    Healt_ = 10;
   */

}



