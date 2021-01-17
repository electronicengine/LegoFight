// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include <vector>
#include "Engine/Engine.h"
#include "LegoCarChasis.h"


// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;


    Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Brick"));
    Brick->SetupAttachment(RootComponent);
    Brick->SetSimulatePhysics(true);

}



// Called when the game starts or when spawned
void ABrick::BeginPlay()
{
	Super::BeginPlay();

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



FVector ABrick::getPlugin(const FVector &ImpactPoint)
{

    if(Plugin_Points.Num() == 0)
        return FVector(0,0,0);

    float distance;
    std::vector<float> distance_array;


    for(int i=0; i<Plugin_Points.Num(); i++)
    {
        distance = calculateDistance(ImpactPoint, (Plugin_Points[i]->GetComponentLocation()));

        distance_array.push_back(distance);
    }

    Current_Plugin_Index = getClosestPluginIndex(distance_array);


    Current_Closest_Constrait = findClosestContrait(ImpactPoint);


    return Plugin_Points[Current_Plugin_Index]->GetComponentLocation();

}



UStaticMesh *ABrick::getBrickMesh()
{
    return Brick_Mesh;
}



FRotator ABrick::getPluginRotation()
{
    if(Plugin_Points.Num()!= 0)
        return Plugin_Points[Current_Plugin_Index]->GetComponentRotation();
    else
        return FRotator(0,0,0);
}



void ABrick::setLegoCarOwner(ALegoCarChasis *Car)
{
    if(Car != nullptr)
        Owner_Car = Car;
}



void ABrick::plugTheBrick(ABrick *Object, const FRotator &OffsetRotation)
{

    FVector plug_location = Plugin_Points[Current_Plugin_Index]->GetComponentLocation();

    if(Plugin_Sockets.Num() == 0)
        return;

    if(Cast<AWeapon>(Object))
    {
        if(Owner_Car != nullptr)
        Owner_Car->addWeaponToInventory(Cast<AWeapon>(Object));
    }

    const FDetachmentTransformRules &attachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
                                                           EDetachmentRule::KeepWorld,
                                                           EDetachmentRule::KeepWorld, true);
    Object->DetachFromActor(attachment_rules);
    Object->enablePhysics(true);
    Object->SetActorEnableCollision(true);

    if(Object->Sub_Type == Semi)
        plug_location += FVector(0,0, -19.0f);

    Object->SetActorLocationAndRotation(plug_location,
                                        Brick->GetComponentRotation() + OffsetRotation,
                                        false, 0, ETeleportType::None);

    Current_Closest_Constrait->SetConstrainedComponents(Brick, FName("None"), Object->Brick, FName("None"));

    if(Owner_Car != nullptr)
        Object->setLegoCarOwner(Owner_Car);

}




void ABrick::highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
                             ABrick *Interactable_Brick, const FHitResult &OutHit, const FRotator &OffetRotation)
{

    FVector plugin = getPlugin(OutHit.ImpactPoint);
    FRotator plugin_rotation = getPluginRotation() + OffetRotation;

    Ghost_Brick->SetStaticMesh(Interactable_Brick->getBrickMesh());

    if(Interactable_Brick->Sub_Type == Semi)
        plugin += FVector(0,0, -19.0f);

    if(plugin != FVector(0,0,0))
    {

        Ghost_Brick->SetVisibility(true);

        Ghost_Brick->SetMaterial(0, Possible_Material);
        Ghost_Brick->SetWorldLocationAndRotation(plugin,
                                                     plugin_rotation);

    }
    else
    {

        Ghost_Brick->SetVisibility(false);
    }

}



float ABrick::calculateDistance(const FVector &Vector1, const FVector &Vector2)
{
    FVector delta = Vector1 - Vector2;

    return delta.Size();
}



int ABrick::getClosestPluginIndex(const std::vector<float> &Array)
{

    float dist = 4000000;
    int index = 0;

    for(int i=0; i<Array.size(); i++)
    {
        if(Array[i] < dist)
        {
            index = i;
            dist = Array[i];
        }
    }

    return index;


}



UPhysicsConstraintComponent *ABrick::findClosestContrait(const FVector &Location)
{
    float distance = 100000;
    std::vector<float> distance_array;
    int index = 0;


    for(int i=0; i<Plugin_Sockets.Num(); i++)
    {
        distance = calculateDistance(Location, Plugin_Sockets[i]->GetComponentLocation());
        distance_array.push_back(distance);
    }


    index = getClosestPluginIndex(distance_array);

//    if(Plugin_Sockets[index]->IsActive())
//        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, TEXT("Socket Active ") + FString::FromInt(index));
//    else
//        GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Socket InActive ") + FString::FromInt(index));


    return Plugin_Sockets[index];
}

