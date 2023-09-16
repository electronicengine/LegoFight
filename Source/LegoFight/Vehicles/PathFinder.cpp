// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"
#include "EnemyLegoVehicle.h"
#include "Engine/Engine.h"

// Sets default values
APathFinder::APathFinder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APathFinder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APathFinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APathFinder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void APathFinder::updateOwnerLocation(const FVector &Location)
{
    float distance = (GetActorLocation() - Location).Size();

    if(distance >= 500000)
        GetCharacterMovement()->Velocity = GetCharacterMovement()->Velocity / 1.5f;
}



void APathFinder::setOwnVehicle(AEnemyLegoVehicle *Vehicle)
{
    Own_Vehicle = Vehicle;
}
