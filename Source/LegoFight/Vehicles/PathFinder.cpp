// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"
#include "EnemyLegoVehicle.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "EnemyVehicleAIController.h"

// Sets default values
APathFinder::APathFinder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshContainer(TEXT("SkeletalMesh'/Game/vehicles/skeleton/lego_car1.lego_car1'"));
	if (MeshContainer.Succeeded())
	{
		
		GetMesh()->SetSkeletalMesh(MeshContainer.Object);
	}

	GetMesh()->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	GetMesh()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionProfileName("IgnoreOnlyPawn");

	// Set the Auto Possess AI property
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AEnemyVehicleAIController::StaticClass();
	

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

    UCharacterMovementComponent* movement_comp = GetCharacterMovement();
    if(distance >= 500000)
        movement_comp->Velocity = movement_comp->Velocity / 1.5f;
}



void APathFinder::setOwnVehicle(AEnemyLegoVehicle *Vehicle)
{
    Own_Vehicle = Vehicle;
}
