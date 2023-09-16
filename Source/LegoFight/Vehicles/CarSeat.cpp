// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSeat.h"




// Sets default values
ACarSeat::ACarSeat()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Type_ = CarSeat2x2;
    Sub_Type = Comp;


    Current_Plugin_Index = 0;

}



// Called when the game starts or when spawned
void ACarSeat::BeginPlay()
{
    Super::BeginPlay();

}



void ACarSeat::Tick(float DeltaTime)
{

}
