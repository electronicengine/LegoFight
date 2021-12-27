// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego2x1Comp.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"



ALego2x1Comp::ALego2x1Comp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_2x1CompDest.BP_2x1CompDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_2x1_all.lego_2x1_all'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(0,-25, 4));
    Collision_Box->SetRelativeScale3D(FVector(0.77f, 1.55f, 0.64f));

    setupPluginPoints(FVector(25, -75, 50.7f), 5, 3);


    Type_ = Lego2x1_Comp;
    Sub_Type = Comp;

    Current_Plugin_Index = 0;
    Healt_= 20;
}



