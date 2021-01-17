// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"



AFireWeapon::AFireWeapon()
{
    Type_ = Lego_Fire_Weapon;


    Barrel_ = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
    Barrel_->SetupAttachment(Brick);



}



void AFireWeapon::fire()
{
    ABullet *bullet_ptr;
    FVector spawn_location = Barrel_->GetComponentLocation();
    FRotator spawn_rotation = Barrel_->GetComponentRotation();

    bullet_ptr = GetWorld()->SpawnActor<ABullet>(Bullet_Container,  spawn_location, spawn_rotation);

    if(bullet_ptr != nullptr)
        bullet_ptr->addFireImpulse(Barrel_->GetComponentRotation().Vector(), 90000);

}
