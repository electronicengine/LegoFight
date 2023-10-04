// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildPlate.h"

// Sets default values
ABuildPlate::ABuildPlate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    

    Static_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));
    SetRootComponent(Static_Mesh);
    Static_Mesh->SetSimulatePhysics(true);
    Static_Mesh->SetCollisionProfileName(FName("BlockAll"));

	setupPluginPoints(FVector(0, 0, 30.7f), 19, 19);
}

USceneComponent* ABuildPlate::CreatePluginPoint(FString Name)
{
    USceneComponent* scene;
    scene = CreateDefaultSubobject<USceneComponent>(FName(Name));
    scene->SetupAttachment(RootComponent);
    return scene;
}



