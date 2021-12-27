// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include <vector>
#include "Engine/Engine.h"
#include "Vehicles/LegoCarChasis.h"
#include "Bricks/Lego1x1Comp.h"


// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;


    Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));
    SetRootComponent(Brick);
    Brick->SetSimulatePhysics(true);
    Brick->SetCollisionProfileName(FName("OverlapAll"));

    Collision_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision_Box"));
    Collision_Box->SetupAttachment(Brick);
//    Collision_Box->SetSimulatePhysics(true);
    Collision_Box->SetNotifyRigidBodyCollision(true);
    Collision_Box->SetCollisionProfileName(FName("BlockAll"));
    plugged = false;

    Collision_Box->OnComponentHit.AddDynamic(this, &ABrick::OnHit);


}


// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();
    First_Hit = true;

}


USceneComponent *ABrick::CreatePluginPoint(FString Name)
{
    USceneComponent *scene;
    scene = CreateDefaultSubobject<USceneComponent>(FName(Name));
    scene->SetupAttachment(Brick);
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
    Collision_Box->SetSimulatePhysics(false);

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

    ADestrictable *dest_ptr;
    FVector spawn_location = Brick->GetComponentLocation();
    FRotator spawn_rotation = Brick->GetComponentRotation();

    dest_ptr = GetWorld()->SpawnActor<ADestrictable>(Destructible_Container,  spawn_location, spawn_rotation);
    GEngine->AddOnScreenDebugMessage(-1 ,5, FColor::Green, FString::SanitizeFloat(Plugged_Bricks_OnIt.size()));


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




void ABrick::setLegoCarOwner(ALegoCarChasis *Car)
{
    if(Car != nullptr)
        Owner_Car = Car;
}




void ABrick::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{

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






