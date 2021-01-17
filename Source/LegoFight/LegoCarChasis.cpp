// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoCarChasis.h"
#include "WheeledVehicleMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"

#include "GameFramework/PlayerController.h"



ALegoCarChasis::ALegoCarChasis() : Socket_Number_Width(10), Socket_Number_Height(22), Socket_Number(Socket_Number_Height * Socket_Number_Width),
    Plugin_Number_Width(11),Plugin_Number_Height(23), Plugin_Number(Plugin_Number_Width * Plugin_Number_Height)
{



    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);


    FString constrait_name("Plug");
    UPhysicsConstraintComponent *constrait;


    for(int i=0; i<1; i++)
    {

        constrait_name = "Plug";
        constrait_name.AppendInt(i);

        constrait = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(constrait_name));
        constrait->SetupAttachment(GetMesh());
        constrait->SetDisableCollision(true);

        constrait->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
        constrait->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
        constrait->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

        Plugin_Sockets.Push(constrait);
    }

    FString scene_name("scene");
    USceneComponent *scene;

    for(int i=0; i<Plugin_Number; i++)
    {

        scene_name = "Scene";
        scene_name.AppendInt(i);

        scene = CreateDefaultSubobject<USceneComponent>(FName(scene_name));
        scene->SetupAttachment(GetMesh());


        Plugin_Points.Push(scene);
    }



    FVector Begining_Pivot = FVector(12.5f, -12.5f, 37.7f);
    FVector Offset = FVector(0,0,-27);
    int  counter = 0;

//    // place Plugin_Sockets
//    for(int i=0; i<Socket_Number_Height; i++)
//    {
//        for(int k = 0; k<Socket_Number_Width; k++)
//        {
//            Plugin_Sockets[counter++]->SetRelativeLocation(Begining_Pivot + Offset);
//            Offset.Y += BRICK_LENGHT / 2;
//        }

//        Offset.X -= BRICK_LENGHT / 2;
//        Offset.Y = 0;
//    }

    Begining_Pivot = FVector(258.89f, -122.55f, 37.7f);
    Offset = FVector(0,0,0);
    counter = 0;

    // place Plugin Pivots
    for(int i=0; i<Plugin_Number_Height; i++)
    {
        for(int k = 0; k<Plugin_Number_Width; k++)
        {
            Plugin_Points[counter++]->SetRelativeLocation(Begining_Pivot + Offset);
            Offset.Y += (BRICK_LENGHT / 2);
        }

        Offset.X -= (BRICK_LENGHT / 2);
        Offset.Y = 0;
    }


    Current_Plugin_Index = 0;
    Car_Seat = nullptr;

}



void ALegoCarChasis::BeginPlay()
{
    Super::BeginPlay();


//    GetMesh()->SetSimulatePhysics(false);



}



void ALegoCarChasis::enterCar(ALegoCharacter *LegoChar)
{
    if(LegoChar != nullptr && Car_Seat != nullptr)
    {

        Passenger_ = LegoChar;

        AController* lego_man_controller = Passenger_->GetController();

        Passenger_->SetActorRotation(Car_Seat->GetActorRotation());

        Passenger_->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
        Passenger_->GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);


        lego_man_controller->Possess(this);

        Passenger_->AttachToActor(Car_Seat, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
                                                                         EAttachmentRule::KeepWorld,
                                                                         EAttachmentRule::KeepWorld, true), TEXT("seat"));
    }

//    GetMesh()->SetSimulatePhysics(true);

}



void ALegoCarChasis::exitCar()
{

    AController* car_controller = this->GetController();

    car_controller->Possess(Passenger_);

    const FDetachmentTransformRules &attachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
                                                       EDetachmentRule::KeepWorld,
                                                       EDetachmentRule::KeepWorld, true);
    Passenger_->DetachFromActor(attachment_rules);

    Passenger_->exitedFromCar();

//    GetMesh()->SetSimulatePhysics(false);

}



void ALegoCarChasis::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ALegoCarChasis::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALegoCarChasis::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &ALegoCarChasis::turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ALegoCarChasis::lookUp);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALegoCarChasis::fire);
    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ALegoCarChasis::equip);
}



void ALegoCarChasis::moveForward(float Value)
{
    GetVehicleMovement()->SetThrottleInput(Value);
}



void ALegoCarChasis::moveRight(float Value)
{
    GetVehicleMovement()->SetSteeringInput(Value);
}



void ALegoCarChasis::turn(float Value)
{
    AddControllerYawInput(Value );
}



void ALegoCarChasis::lookUp(float Value)
{
    AddControllerPitchInput(Value);
}



void ALegoCarChasis::fire()
{
    for(int i=0; i<Armed_Fire_Weapons.Num(); i++)
    {
        if(Armed_Fire_Weapons[i] != nullptr)
            Armed_Fire_Weapons[i]->fire();
    }
}



void ALegoCarChasis::equip()
{
    exitCar();
}



FVector ALegoCarChasis::getPlugin(const FVector &ImpactPoint)
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


//    Current_Closest_Constrait = findClosestContrait(Plugin_Points[Current_Plugin_Index]->GetComponentLocation());


    return Plugin_Points[Current_Plugin_Index]->GetComponentLocation();

}



FRotator ALegoCarChasis::getPluginRotation()
{
    return GetActorRotation();
}



void ALegoCarChasis::addWeaponToInventory(AWeapon *Weapon)
{
    if(Weapon->Type_ == Lego_Fire_Weapon)
        Armed_Fire_Weapons.Push(Cast<AFireWeapon>(Weapon));
    else
        Armed_Melee_Weapons.Push(Cast<AMeleeWeapon>(Weapon));
}



void ALegoCarChasis::plugTheBrick(ABrick *Object, const FRotator &OffsetRotation)
{
    FVector plugin_location = Plugin_Points[Current_Plugin_Index]->GetComponentLocation();

    if(Plugin_Sockets.Num() == 0)
        return;

    if(Object->Type_ == Lego_Car_Seat)
        Car_Seat = Cast<ACarSeat>(Object);

    if(Cast<AWeapon>(Object))
        addWeaponToInventory(Cast<AWeapon>(Object));


    Object->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld,
                                                      EDetachmentRule::KeepWorld,
                                                      EDetachmentRule::KeepWorld, true));
    Object->enablePhysics(true);
    Object->SetActorEnableCollision(true);


    if(Object->Sub_Type == Semi)
        plugin_location += FVector(0,0, -19.0f);

    Object->SetActorLocationAndRotation(plugin_location,
                                        (GetActorRotation() + OffsetRotation),
                                        false, 0, ETeleportType::None);

    Object->enablePhysics(false);
    Object->SetActorEnableCollision(true);

    Object->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld,
                                                           EAttachmentRule::KeepWorld,
                                                           EAttachmentRule::KeepWorld, true));

    Object->setLegoCarOwner(this);

}



float ALegoCarChasis::calculateDistance(const FVector &Vector1, const FVector &Vector2)
{
    FVector delta = Vector1 - Vector2;

    return delta.Size();
}



int ALegoCarChasis::getClosestPluginIndex(const std::vector<float> &Array)
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



UPhysicsConstraintComponent *ALegoCarChasis::findClosestContrait(const FVector &Location)
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

    return Plugin_Sockets[index];
}



void ALegoCarChasis::highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
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
