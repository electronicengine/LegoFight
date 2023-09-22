// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = false;


    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Brick);

    Sub_Type = Semi;
    Healt_ = 900000;


}



void AWeapon::useWeapon()
{



    APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
    controller->SetViewTargetWithBlend(this);

//        controller->Possess(Cast<APawn>(this));




}

void AWeapon::fire()
{
    Barrel_Location = Brick->GetSocketLocation("barrel");
    Barrel_Rotation = Brick->GetSocketRotation("barrel");

    ABullet* bullet_ptr;

    bullet_ptr = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), Barrel_Location, Barrel_Rotation);
    FVector direction = Barrel_Rotation.Vector();
    
    if (bullet_ptr != nullptr)
        bullet_ptr->addFireImpulse(direction, 50000);
}

