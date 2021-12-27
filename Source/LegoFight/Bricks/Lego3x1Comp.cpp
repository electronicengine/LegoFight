// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego3x1Comp.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"




ALego3x1Comp::ALego3x1Comp()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_3x1CompDest.BP_3x1CompDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_3x1_all.lego_3x1_all'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(0,-50, 4));
    Collision_Box->SetRelativeScale3D(FVector(0.77f, 2.31f, 0.64f));

    Type_ = Lego3x1_Comp;
    Sub_Type = Comp;


    setupPluginPoints(FVector(25, -125, 50.7), 7, 3);
    Current_Plugin_Index = 0;
    Healt_ = 40;

}


