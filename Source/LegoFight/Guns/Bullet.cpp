// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "../Brick.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "../Vehicles/LegoCarChasis.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Bullet_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_Mesh"));
    SetRootComponent(Bullet_Mesh);
    Bullet_Mesh->SetSimulatePhysics(true);
    Bullet_Mesh->SetNotifyRigidBodyCollision(true);
    Bullet_Mesh->SetCollisionProfileName(FName("BlockAll"));


    static ConstructorHelpers::FObjectFinder<UStaticMesh>
            static_mesh_asset(TEXT("StaticMesh'/Game/bricks/meshes/High2x2.High2x2'"));
    UStaticMesh* mesh = static_mesh_asset.Object;
    mesh = static_mesh_asset.Object;
    Bullet_Mesh->SetStaticMesh(mesh);

    //static ConstructorHelpers::FObjectFinder<UMaterial>
    //        material_asset(TEXT("Material'/Game/Guns/materials/bulet_material.bulet_material'"));
    //UMaterial *material = material_asset.Object;
    //Bullet_Mesh->SetMaterial(0, material);

    Bullet_Mesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));


    InitialLifeSpan = 2;

    Bullet_Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
    First_Hit = false;


    static ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("/Game/effects/effect"));
    Particle_Effect = particle.Object;

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset(TEXT("Material'/Game/Guns/materials/bullet_material.bullet_material'"));

    Bullet_Mesh->SetMaterial(0, material_asset.Object);
}



void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    ABrick* brick = Cast<ABrick>(OtherActor);
    ALegoCarChasis * vehicle = Cast<ALegoCarChasis>(OtherActor);
    if (!First_Hit) {
        if (brick)
        {
            brick->addDamage(3);
        }
        else if (vehicle) {
            vehicle->addDamage(3);
        }
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_Effect, GetActorLocation());
        First_Hit = true;
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

void ABullet::spawnparticle()
{

    // Load the particle system blueprint




}

