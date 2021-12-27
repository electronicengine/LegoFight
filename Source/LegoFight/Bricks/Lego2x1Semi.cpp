// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego2x1Semi.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"



ALego2x1Semi::ALego2x1Semi()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;


    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_2x1SemiDest.BP_2x1SemiDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_2x1_semi.lego_2x1_semi'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(25,0, 2));
    Collision_Box->SetRelativeScale3D(FVector(1.55f, 0.77f, 0.12f));

    Type_ = Lego2x1_Semi;
    Sub_Type = Semi;


    setupPluginPoints(FVector(75, -25, 31.5f), 3, 5);
    Current_Plugin_Index = 0;
    Healt_ = 20;
}
