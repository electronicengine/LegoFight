// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego1x1Triangle.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"




ALego1x1Triangle::ALego1x1Triangle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FClassFinder<ADestrictable>
             DestructibleBP(TEXT("Blueprint'/Game/bricks/destructibles/BP_1x1TriangleDest.BP_1x1TriangleDest_C'"));
    Destructible_Container = DestructibleBP.Class;

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_triangle.lego_triangle'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    Brick_Mesh = static_mesh_asset.Object;
    Brick->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *material = material_asset.Object;
    Brick->SetMaterial(0, material);

    Collision_Box->SetRelativeLocation(FVector(0,0,4));
    Collision_Box->SetRelativeScale3D(FVector(0.77f, 0.77f, 0.64f));


    Type_ = Lego1x1_Triangle;
    Sub_Type = Comp;
    Healt_ = 10;

}


