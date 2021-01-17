// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego1x1Triangle.h"


// Sets default values
ALego1x1Triangle::ALego1x1Triangle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Type_ = Lego1x1_Triangle;
    Sub_Type = Comp;

}


// Called when the game starts or when spawned
void ALego1x1Triangle::BeginPlay()
{
    Super::BeginPlay();

}



// Called every frame
void ALego1x1Triangle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

