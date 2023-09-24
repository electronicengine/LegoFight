// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include <vector>
#include "Engine/Engine.h"
#include "Vehicles/LegoCarChasis.h"
#include "LegoFightGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/UnrealString.h"

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
    GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, TEXT("breakBrick"));
    Healt_ -= Value;

    if(Healt_ <= 0)
        breakBrick();


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

void ABrick::setBrickTypeOptions(ItemOptions&Options)
{
    float pivot_loc_x;
    float pivot_loc_y;
    float pivot_loc_z;
    int pivot_width;
    int pivot_height;
    int offset;


    Brick->SetStaticMesh(Options.Mesh);
    Brick_Mesh = Options.Mesh;
    Brick->SetMaterial(0, Options.Material);
    Material = Options.Material;


    FVector Origin;
    FVector BoxExtent;
    float SphereRadius;

    UKismetSystemLibrary::GetComponentBounds(Brick, Origin, BoxExtent, SphereRadius);

    GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Cyan, FString("Origin: ") + Origin.ToString());
    GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, FString("Extend: ") + BoxExtent.ToString());
    GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Green, FString("Radius: ") + FString::SanitizeFloat(SphereRadius));

    if (Options.Name.Find(WEAPON_APPENDIX) > 0) {

        Height_Offset = 20;

    }else if (Options.Name.Find(SIDED_APPENDIX) > 0) {

        setupPluginPoints(FVector(25, -25, 30.7f), 3, 3);
        setupSidePlugPoints(FVector(50.1f, 55, -13.85f), 3, 9);
        Height_Offset = 20;
    }
    else {

        pivot_loc_x = Origin.X + BoxExtent.X;
        pivot_loc_y = (Origin.Y + BoxExtent.Y) * -1;
        pivot_loc_z = Origin.Z + BoxExtent.Z + 15;
        pivot_width = (BoxExtent.Y / 12.5) + 1;
        pivot_height = (BoxExtent.X / 12.5) + 1;
        offset = 2 * (int)(17 - BoxExtent.Z) + 2;

        setupPluginPoints(FVector(pivot_loc_x, pivot_loc_y, pivot_loc_z), pivot_width, pivot_height);
        Height_Offset = offset;

    }




    Destructible_Container = ADestrictable::StaticClass();


    //Type_ = Options.Type;
    //Sub_Type = Options.SubType;
    Healt_ = Options.Health;

    Current_Plugin_Index = 0;


    Brick->CreateDynamicMaterialInstance(0);
    Brick->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(Options.Color));
    Brick_Color = FVector(Options.Color);

    Brick->OnComponentHit.AddDynamic(this, &ABrick::OnHit);

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
        AWeapon *weapon = Cast<AWeapon>(this);

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






