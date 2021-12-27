// Fill out your copyright notice in the Description page of Project Settings.


#include "Destrictable.h"


// Sets default values
ADestrictable::ADestrictable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestrictable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestrictable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

