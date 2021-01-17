// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Engine/Engine.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Bullet_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet_Mesh"));
    Bullet_Mesh->SetupAttachment(RootComponent);
    Bullet_Mesh->SetSimulatePhysics(true);

    InitialLifeSpan = 30;
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

