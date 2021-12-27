// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego3x2Semi.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"



ALego3x2Semi::ALego3x2Semi()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_3x2SemiDest.BP_3x2SemiDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_3x2_semi.lego_3x2_semi'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(50, -25, 2));
    Collision_Box->SetRelativeScale3D(FVector(2.31f, 1.55f, 0.12f));

    Type_ = Lego3x2_Semi;
    Sub_Type = Semi;

    setupPluginPoints(FVector(125, -75, 31.5f), 5, 7);

    Current_Plugin_Index = 0;
    Healt_ = 50;

}
