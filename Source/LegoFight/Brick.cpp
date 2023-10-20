// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include <vector>
#include "Engine/Engine.h"
#include "Vehicles/LegoCarChasis.h"
#include "LegoFightGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/UnrealString.h"
#include "Engine/StaticMeshSocket.h"

#include "Materials/MaterialInstanceDynamic.h"



// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Destructible_Container = ADestrictable::StaticClass();

    Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));
    SetRootComponent(Brick);
    Brick->SetSimulatePhysics(true);
    Brick->SetCollisionProfileName(FName("BlockAll"));
    //Brick->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);


    plugged = false;





    for (int i = 0; i < 100; i++) {

        FString scene_name = "sidescene";
        scene_name.AppendInt(i);

        USceneComponent* scene;
        scene = CreateDefaultSubobject<USceneComponent>(FName(scene_name));
        scene->SetupAttachment(Brick);
        plugs.push(scene);

    }



}


// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
    First_Hit = true;

}


USceneComponent* ABrick::CreatePluginPoint(FString Name)
{
    USceneComponent* scene;
    //scene = CreateDefaultSubobject<USceneComponent>(FName(Name));
    //scene->SetupAttachment(Brick);

    /**/
    if (Name == "last")
    {
        while (plugs.size() > 0) {
            scene = plugs.front();
            scene->DestroyComponent();
            plugs.pop();
        }

        return nullptr;
    }
       
    scene = plugs.front();
    plugs.pop();
   

    return scene;
}



// Called every frame
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABrick::enablePhysics(bool Value)
{
    if(Brick)
        Brick->SetSimulatePhysics(Value);
}

void ABrick::setCollisionProfile(FString Profile)
{
    if (Brick)
        Brick->SetCollisionProfileName(FName(Profile));

}




UStaticMesh *ABrick::getBrickMesh()
{
    return Brick_Mesh;
}



void ABrick::addDamage(int Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("breakBrick"));
    Healt_ -= Value;

    if(Healt_ <= 0)
        breakBrick();


}



void ABrick::breakBrick()
{

    ADestrictable *dest_ptr;
    FVector spawn_location = Brick->GetComponentLocation();
    FRotator spawn_rotation = Brick->GetComponentRotation();

    //GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, Brick_Name);

    dest_ptr = GetWorld()->SpawnActor<ADestrictable>(ADestrictable::StaticClass(), spawn_location, spawn_rotation);
    dest_ptr->setMesh(Brick_Name);
    dest_ptr->setColor(Brick_Color);

//    const FDetachmentTransformRules &detachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
//                                                           EDetachmentRule::KeepWorld,
//                                                           EDetachmentRule::KeepWorld, true);
    
    
    
    detachItemsOnIt();

    

    Destroy();
}

void ABrick::setMaterialColor(FLinearColor Color)
{
    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Color));
    Brick_Color = FVector(Color);
}

void ABrick::setBrickTypeOptions(ItemOptions&Options)
{
    //float pivot_loc_x;
    //float pivot_loc_y;
    //float pivot_loc_z;
    //int pivot_width;
    //int pivot_height;
    //int offset;


    Brick->SetStaticMesh(Options.Mesh);
    Brick_Mesh = Options.Mesh;
    Brick->SetMaterialByName(FName("main"), Options.Material);
    Material = Options.Material;


    FVector Origin;
    FVector BoxExtent;
    float SphereRadius;

    UKismetSystemLibrary::GetComponentBounds(Brick, Origin, BoxExtent, SphereRadius);


    TArray<FName> names = Brick->GetAllSocketNames();

    for (FName name : names) {

        FVector pivot = Brick->GetSocketLocation(name);

        FString tag = Brick->GetSocketByName(name)->Tag;
        FString width = tag.Mid(0, tag.Find(":"));


        int plug_width = FCString::Atoi(*width.Mid(0, width.Find("x")));
        int plug_height = FCString::Atoi(*width.Right(width.Find("x")));

        if (name.ToString().Find("side") < 0) {
            setupPluginPoints(pivot, plug_width, plug_height);
            Height_Offset = FCString::Atoi(*tag.Right(tag.Find(":") - 1));
        }
        else {
            setupSidePlugPoints(pivot, plug_width, plug_height);
        }

    }


    Destructible_Container = ADestrictable::StaticClass();


    //Type_ = Options.Type;
    //Sub_Type = Options.SubType;
    Healt_ = Options.Health;

    Current_Plugin_Index = 0;

    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Options.Color));
    Brick_Color = FVector(Options.Color);
    Brick_Name = Options.Name;

}




void ABrick::setLegoCarOwner(ALegoCarChasis *Car)
{
    if(Car != nullptr)
        Owner_Car = Car;
}




