// Fill out your copyright notice in the Description page of Project Settings.


#include "FireWeapon.h"



AFireWeapon::AFireWeapon()
{
    Type_ = (BrickType)0;

    Barrel_ = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
    Barrel_->SetupAttachment(Brick);

}



void AFireWeapon::fire()
{
    ABullet *bullet_ptr;
    FVector spawn_location = Barrel_->GetComponentLocation();
    FRotator spawn_rotation = Barrel_->GetComponentRotation();

    static double new_time = FPlatformTime::Seconds();
    static double last_time = 0;


    new_time = FPlatformTime::Seconds();

    if(new_time - last_time > 1.0f)
    {
        bullet_ptr = GetWorld()->SpawnActor<ABullet>(Bullet_Container,  spawn_location, spawn_rotation);

        if(bullet_ptr != nullptr)
            bullet_ptr->addFireImpulse(Barrel_->GetComponentRotation().Vector(), 400000);

        last_time = new_time;


    }



}



FVector AFireWeapon::getBarrelVector()
{
    return Barrel_->GetComponentRotation().Vector();
}
