// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "../Brick.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Bullet_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_Mesh"));
    SetRootComponent(Bullet_Mesh);
    Bullet_Mesh->SetSimulatePhysics(true);

    Bullet_Mesh->SetCollisionProfileName(FName("BlockAll"));

    Bullet_Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/lego_1x1_all.lego_1x1_all'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    mesh = static_mesh_asset.Object;
    Bullet_Mesh->SetStaticMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset(TEXT("Material'/Game/Guns/materials/bulet_material.bulet_material'"));
    UMaterial *material = material_asset.Object;
    Bullet_Mesh->SetMaterial(0, material);

    Bullet_Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));


    InitialLifeSpan = 10;
}



void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    ABrick* brick = Cast<ABrick>(OtherActor);

    if(brick != nullptr )
    {
        brick->addDamage(10);
//        Destroy();

    }

}



// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABullet::addFireImpulse(FVector Direction ,float Strenght)
{
    FVector n = Direction * Strenght;

    Bullet_Mesh->AddImpulse(n);
}

