// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstraitBrick.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/UnrealString.h"
#include "Engine/StaticMeshSocket.h"


AConstraitBrick::AConstraitBrick() {



    Add = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Add"));
    Add->SetMobility(EComponentMobility::Movable);  // Set as movable

    Add->SetupAttachment(RootComponent);


    Add->SetSimulatePhysics(true);
    Brick->SetSimulatePhysics(true);
    Brick->SetMobility(EComponentMobility::Movable);  // Set as movable


    USceneComponent* scene;

    while (plugs.size() > 0) {
        scene = plugs.front();
        scene->DestroyComponent();
        plugs.pop();
    }

    for (int i = 0; i < 10; i++) {

        FString scene_name = "constrait";
        scene_name.AppendInt(i);

        scene = CreateDefaultSubobject<USceneComponent>(FName(scene_name));
        scene->SetupAttachment(Add);
        plugs.push(scene);

    }

    Healt_ = 1000;
    Height_Offset = 20;
    Machine_Running = false;
    Cable_Hook = false;
}

void AConstraitBrick::setHeightOffset(int Offset)
{
    Height_Offset = Offset;
}

bool AConstraitBrick::turnOnOffMachine()
{

    if (Machine_Running) {
        Machine_Running = false;
    }
    else {
        Machine_Running = true;

    }

    return Machine_Running;
}

void AConstraitBrick::setGhostComponent(UStaticMesh* Mesh)
{
    Ghost_Mesh = Mesh;
}

void AConstraitBrick::BeginPlay()
{
    Super::BeginPlay();
    FVector pivot = FVector(0,0,30.7f);

    TArray<FName> names = Brick->GetAllSocketNames();

    for (FName name : names) {
        FString tag = Add->GetSocketByName(name)->Tag;
        FString width = tag.Mid(0, tag.Find(":"));


        int plug_width = FCString::Atoi(*width.Mid(0, width.Find("x")));
        int plug_height = FCString::Atoi(*width.Right(width.Find("x")));

        setupPluginPoints(pivot, plug_width, plug_height);
    }
   

}

void AConstraitBrick::setMaterialColor(FLinearColor Color)
{

    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Color));
    Brick_Color = FVector(Color);

    Add->CreateDynamicMaterialInstance(0);
    Add->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Color));
}
