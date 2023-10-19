// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoCarChasis.h"
#include "WheeledVehicleMovementComponent.h"
#include "Components/MeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "VehicleAnimInstance.h"
#include "../Widgets/CharacterWidget.h"
#include "../LegoFightGameInstance.h"
#include "../Interfaces/InteractInterface.h"



ALegoCarChasis::ALegoCarChasis()

{


    //static ConstructorHelpers::FObjectFinder<USkeletalMesh>
    //        mesh_asset(TEXT("SkeletalMesh'/Game/vehicles/skeleton/lego_car_chasis_1.lego_car_chasis_1'"));
    static ConstructorHelpers::FObjectFinder<USkeletalMesh>
        mesh_asset(TEXT("SkeletalMesh'/Game/vehicles/skeleton/lego_car1.lego_car1'"));
    USkeletalMesh* mesh = mesh_asset.Object;
    GetMesh()->SetSkeletalMesh(mesh);

    static ConstructorHelpers::FClassFinder<UAnimInstance>
             anim_bp(TEXT("AnimBlueprint'/Game/vehicles/anim/legocar1_anim.legocar1_anim_C'"));
    GetMesh()->AnimClass = anim_bp.Class;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->bUsePawnControlRotation = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->SetRelativeLocation(FVector(-506.0f, 0, 170.f));

    setupMaterials();
    setupWheels();
    //setupPluginPoints(FVector(258.8, -122.7, 37.7f), 11, 23);
    setupPluginPoints(FVector(85.0f, -12.5f, 30.7f), 3, 9);
    Weapon_Camera_Used = false;
    Current_Plugin_Index = 0;
    Car_Seat = nullptr;
    Current_Camera_Index = 0;
    Team_Id = 1;

    Weapon_Index = 0;
    Plugin_Number_Width = 3;
    Plugin_Number_Height = 9;
    Plugin_Number = Plugin_Number_Width * Plugin_Number_Height;
    Healt = 40;
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
    //tire.AdditionalOffset = FVector(173.104965f, -187.788605f, -57.286331f);
    tire.bDisableSteering = false;
    CTyres.Add(tire);

    tire.WheelClass = front_tire.Class;
    tire.BoneName = FName("front_right");
    //tire.AdditionalOffset = FVector(172.294922f, 190.490829f, -57.286308f);
    tire.bDisableSteering = false;
    CTyres.Add(tire);

    tire.WheelClass = back_tire.Class;
    tire.BoneName = FName("back_left");
    //tire.AdditionalOffset = FVector(-204.372986f, 190.490829f, -57.286308f);
    tire.bDisableSteering = true;
    CTyres.Add(tire);

    tire.WheelClass = back_tire.Class;
    tire.BoneName = FName("back_right");
    //tire.AdditionalOffset = FVector(-204.372986f, -202.227158f, -57.286331f);
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

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset1(TEXT("Material'/Game/vehicles/materials/wheel_material.wheel_material'"));
    UMaterial* wheel_material = material_asset1.Object;


    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset2(TEXT("Material'/Game/vehicles/materials/root_material.root_material'"));
    UMaterial* root_material = material_asset2.Object;



    GetMesh()->SetMaterial(0, root_material);
    GetMesh()->SetMaterial(1, chasis_material);
    GetMesh()->SetMaterial(2, wheel_material);
    //GetMesh()->SetMaterial(3, wheel_material);
    //GetMesh()->SetMaterial(4, wheel_material);

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
    Game_Instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
}




void ALegoCarChasis::jump()
{

    FVector as = GetMesh()->GetForwardVector();
    FVector asd = as.UpVector;

    GetMesh()->AddImpulse(asd * 1000000);
}



void ALegoCarChasis::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ALegoCarChasis::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALegoCarChasis::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &ALegoCarChasis::turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ALegoCarChasis::lookUp);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALegoCarChasis::jump);

    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ALegoCarChasis::equip);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ALegoCarChasis::interact);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALegoCarChasis::fire);

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



}



void ALegoCarChasis::fire()
{

    static int weapon_index;

    if(Weapons.size() != 0)
    {

        if(Weapons[Current_Weapon_Index])
            Weapons[Current_Weapon_Index]->fire();
    }

}





int ALegoCarChasis::removeWeaponToInventory(int Index)
{
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString("removeWeaponToInventory : ") + FString::FromInt(Index));

    if (Weapons.size() > 0) {
        Weapons.erase(Index);
    }

    return 0;
}

int ALegoCarChasis::addWeaponToInventory(AWeapon *Weapon)
{
    Current_Weapon_Index++;

    Weapons.insert(std::make_pair(Current_Weapon_Index, Weapon));

    return Current_Weapon_Index;

}



void ALegoCarChasis::addSeatToCar(ACarSeat *Seat)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f ,FColor::Blue, TEXT("addSeatToCar"));

    if(Seat != nullptr)
        Car_Seat = Seat;
}

void ALegoCarChasis::addDamage(int Value)
{
        Healt -= Value;

        if (Healt <= 0)
            breakBrick();
    
}

void ALegoCarChasis::breakBrick()
{
    ADestrictable* dest_ptr;
    FVector spawn_location = GetMesh()->GetComponentLocation();
    FRotator spawn_rotation = GetMesh()->GetComponentRotation();

    //GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, Brick_Name);

    dest_ptr = GetWorld()->SpawnActor<ADestrictable>(ADestrictable::StaticClass(), spawn_location, spawn_rotation);
    dest_ptr->setMesh("lego_car1");
    //dest_ptr->setColor(Brick_Color);

    //    const FDetachmentTransformRules &detachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
    //                                                           EDetachmentRule::KeepWorld,
    //                                                           EDetachmentRule::KeepWorld, true);



    detachItemsOnIt();

    Destroy();
}




