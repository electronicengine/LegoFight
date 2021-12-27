// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego2x2Comp.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"


ALego2x2Comp::ALego2x2Comp()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_2x2CompDest.BP_2x2CompDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_2x2_all.lego_2x2_all'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(25,-25, 4));
    Collision_Box->SetRelativeScale3D(FVector(1.55f, 1.55f, 0.64f));


    Type_ = Lego2x2_Comp;
    Sub_Type = Comp;

    setupPluginPoints(FVector(75, -75, 50.7f), 5, 5);

    Current_Plugin_Index = 0;
    Healt_ = 40;
}

