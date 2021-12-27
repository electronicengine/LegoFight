// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoCarChasis.h"
#include "WheeledVehicleMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "VehicleAnimInstance.h"



ALegoCarChasis::ALegoCarChasis()

{


    static ConstructorHelpers::FObjectFinder<USkeletalMesh>
            mesh_asset(TEXT("SkeletalMesh'/Game/vehicles/skeleton/lego_car_chasis_1.lego_car_chasis_1'"));
    USkeletalMesh* mesh = mesh_asset.Object;
    GetMesh()->SetSkeletalMesh(mesh);

    static ConstructorHelpers::FClassFinder<UAnimInstance>
             anim_bp(TEXT("AnimBlueprint'/Game/vehicles/anim/BP_LegoCarAnim.BP_LegoCarAnim_C'"));
    GetMesh()->AnimClass = anim_bp.Class;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->SetRelativeLocation(FVector(-506.0f, 0, 170.f));

    setupMaterials();
    setupWheels();
    setupPluginPoints(FVector(258.8, -122.7, 37.7f), 11, 23);
    Weapon_Camera_Used = false;
    Current_Plugin_Index = 0;
    Car_Seat = nullptr;
    Current_Camera_Index = 0;
    Team_Id = 1;

    Plugin_Number_Width = 11;
    Plugin_Number_Height = 23;
    Plugin_Number = Plugin_Number_Width * Plugin_Number_Height;
}



void ALegoCarChasis::setupWheels()
{

    TArray<FWheelSetup> CTyres;
    FWheelSetup tire;
    static ConstructorHelpers::FClassFinder<UVehicleWheel>
             front_tire(TEXT("Blueprint'/Game/vehicles/front_wheel.front_wheel_C'"));
    static ConstructorHelpers::FClassFinder<UVehicleWheel>
             back_tire(TEXT("Blueprint'/Game/vehicles/back_wheel.back_wheel_C'"));

    tire.WheelClass = front_tire.Class;
    tire.BoneName = FName("front_left");
    tire.AdditionalOffset = FVector(173.104965f, -187.788605f, -57.286331f);
    tire.bDisableSteering = false;
    CTyres.Add(tire);

    tire.WheelClass = front_tire.Class;
    tire.BoneName = FName("front_right");
    tire.AdditionalOffset = FVector(172.294922f, 190.490829f, -57.286308f);
    tire.bDisableSteering = false;
    CTyres.Add(tire);

    tire.WheelClass = back_tire.Class;
    tire.BoneName = FName("back_left");
    tire.AdditionalOffset = FVector(-204.372986f, 190.490829f, -57.286308f);
    tire.bDisableSteering = true;
    CTyres.Add(tire);

    tire.WheelClass = back_tire.Class;
    tire.BoneName = FName("back_right");
    tire.AdditionalOffset = FVector(-204.372986f, -202.227158f, -57.286331f);
    tire.bDisableSteering = true;
    CTyres.Add(tire);

    GetVehicleMovementComponent()->WheelSetups = CTyres;
    GetVehicleMovementComponent()->CreateVehicle();
    GetVehicleMovementComponent()->RecreatePhysicsState();
}



void ALegoCarChasis::setupMaterials()
{
    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset0(TEXT("Material'/Game/vehicles/materials/chasis_material.chasis_material'"));
    UMaterial *chasis_material = material_asset0.Object;
    GetMesh()->SetMaterial(0, chasis_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset1(TEXT("Material'/Game/vehicles/materials/wheel_material.wheel_material'"));
    UMaterial *wheel_material = material_asset1.Object;
    GetMesh()->SetMaterial(1, wheel_material);
    GetMesh()->SetMaterial(2, wheel_material);
    GetMesh()->SetMaterial(3, wheel_material);
    GetMesh()->SetMaterial(4, wheel_material);

}



USceneComponent *ALegoCarChasis::CreatePluginPoint(FString Name)
{
    USceneComponent *scene;
    scene = CreateDefaultSubobject<USceneComponent>(FName(Name));
    scene->SetupAttachment(GetMesh());
    return scene;
}



void ALegoCarChasis::BeginPlay()
{
    Super::BeginPlay();


//    GetMesh()->SetSimulatePhysics(false);

    GetVehicleMovement()->SetHandbrakeInput(true);

}



bool ALegoCarChasis::carHasPassenger()
{
    if(Passenger_ == nullptr)
        return false;
    else
        return true;
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

    Passenger_ = nullptr;

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
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALegoCarChasis::interact);

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



void ALegoCarChasis::interact()
{


    if(Current_Camera_Index >= Armed_Fire_Weapons.Num())
    {
        Current_Camera_Index = 0;
        APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
        controller->SetViewTargetWithBlend(Cast<AActor>(this));
        Weapon_Camera_Used = false;

    }
    else
    {
        Armed_Fire_Weapons[Current_Camera_Index]->useWeapon();
        Weapon_Camera_Used = true;
        Current_Camera_Index++;

    }



}



void ALegoCarChasis::fire()
{

    static int weapon_index;

    if(Armed_Fire_Weapons.Num() != 0)
    {

        if(Current_Camera_Index == 0)
            weapon_index = 0;
        else
            weapon_index = Current_Camera_Index - 1;



        if(Armed_Fire_Weapons[weapon_index] != nullptr)
            Armed_Fire_Weapons[weapon_index]->fire();
    }

}



void ALegoCarChasis::equip()
{
    exitCar();
}



void ALegoCarChasis::addWeaponToInventory(AWeapon *Weapon)
{
    if(Weapon->Type_ == Lego_Fire_Weapon)
        Armed_Fire_Weapons.Push(Cast<AFireWeapon>(Weapon));
    else
        Armed_Melee_Weapons.Push(Cast<AMeleeWeapon>(Weapon));
}



void ALegoCarChasis::addSeatToCar(ACarSeat *Seat)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f ,FColor::Blue, TEXT("addSeatToCar"));

    if(Seat != nullptr)
        Car_Seat = Seat;
}



