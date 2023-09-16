// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include <vector>
#include "Engine/Engine.h"
#include "Vehicles/LegoCarChasis.h"
#include "Bricks/Lego1x1Comp.h"
#include "LegoFightGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"



// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    Destructible_Container = ADestrictable::StaticClass();
    Type_ = High2x2;

    Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));
    SetRootComponent(Brick);
    Brick->SetSimulatePhysics(true);
    Brick->SetCollisionProfileName(FName("BlockAll"));
    Brick->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);


    plugged = false;



    Brick->OnComponentHit.AddDynamic(this, &ABrick::OnHit);


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
    Brick->SetSimulatePhysics(Value);
}

void ABrick::setCollisionProfile(FString Profile)
{
    Brick->SetCollisionProfileName(FName(Profile));

}




UStaticMesh *ABrick::getBrickMesh()
{
    return Brick_Mesh;
}



void ABrick::addDamage(int Value)
{

    if(Healt_ == 0)
        breakBrick();
    else
    {
        Healt_ -=Value;

        if(Healt_ <= 0)
            breakBrick();
    }

}



void ABrick::breakBrick()
{
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("breakBrick"));

    ADestrictable *dest_ptr;
    FVector spawn_location = Brick->GetComponentLocation();
    FRotator spawn_rotation = Brick->GetComponentRotation();

    dest_ptr = GetWorld()->SpawnActor<ADestrictable>(Destructible_Container, spawn_location, spawn_rotation);
    dest_ptr->setMesh(Type_);
    dest_ptr->setColor(Brick_Color);

//    const FDetachmentTransformRules &detachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
//                                                           EDetachmentRule::KeepWorld,
//                                                           EDetachmentRule::KeepWorld, true);
    for(int i=0; i<Plugged_Bricks_OnIt.size(); i++)
    {
        if(Plugged_Bricks_OnIt[i] != nullptr)
            Plugged_Bricks_OnIt[i]->releaseAll();
    }


    Destroy();
}

void ABrick::setMaterialColor(FLinearColor Color)
{
    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Color));
    Brick_Color = FVector(Color);
}

void ABrick::setBrickTypeOptions(BrickOptions &Options)
{

    Brick->SetStaticMesh(Options.Mesh);
    Brick_Mesh = Options.Mesh;
    Brick->SetMaterial(0, Options.Material);
    Material = Options.Material;


    Destructible_Container = ADestrictable::StaticClass();


    //Type_ = Options.Type;
    //Sub_Type = Options.SubType;
    Healt_ = Options.Health;

    Current_Plugin_Index = 0;

    setupPluginPoints(Options.Plug_Pivot_Location, Options.Pivot_Width, Options.Pivot_Height);
    Height_Offset = Options.Height_Offset;

    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Options.Color));
    Brick_Color = FVector(Options.Color);
}




void ABrick::setLegoCarOwner(ALegoCarChasis *Car)
{
    if(Car != nullptr)
        Owner_Car = Car;
}




void ABrick::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{

    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("First_Hit"));

    if(First_Hit != false)
    {

        ABrick *below_brick = Cast<ABrick>(OtherActor);

        if(below_brick != nullptr)
        {
//            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("cast"));

//            autoPlugin(below_brick);


        }
        else
        {
            ALegoCarChasis *car = Cast<ALegoCarChasis>(OtherActor);

//            if(car != nullptr)
//                autoPlugin(car);

        }

        First_Hit = false;
    }
    else
    {
        AMeleeWeapon *weapon = Cast<AMeleeWeapon>(this);

        if(weapon != nullptr)
        {
            ABrick *below_brick = Cast<ABrick>(OtherActor);

            if(below_brick != nullptr)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("hit meale weapon "));

                below_brick->addDamage(10);
            }

        }
    }



}






