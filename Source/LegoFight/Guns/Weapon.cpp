// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"
#include "Kismet/GameplayStatics.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Brick);

    Healt_ = 900000;
    


}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (checkWeaponDetached())
        return;

    if (Owner_Car) {
        if (Owner_Car->carHasPassenger()) {

            // Get the player controller
            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                // Get the player's camera rotation
                FRotator CameraRotation = PlayerController->GetControlRotation();

                // Set the actor's rotation to match the camera rotation
                SetActorRotation(CameraRotation);
            }
        }
    }

}

void AWeapon::aimToRotation(FRotator& Rot)
{
    SetActorRotation(Rot);
}

void AWeapon::removeOwner()
{
    if (Owner_Car)
        Owner_Car->removeWeaponToInventory(Weapon_Index);

    Owner_Car = nullptr;

    
    Brick->SetSimulatePhysics(true);
    Brick->SetCollisionProfileName(FName("BlockAll"));

    GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("BlockAll"));


}

bool AWeapon::checkWeaponDetached()
{
    if (Owner_Car) {
        if ((Owner_Car->GetActorLocation() - GetActorLocation()).Size() >= 200) {
            removeOwner();
            return true;
        }
        else {
            return false;
        }
    }
    return false;
}


void AWeapon::useWeapon()
{



    APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
    controller->SetViewTargetWithBlend(this);

//        controller->Possess(Cast<APawn>(this));



}

void AWeapon::fire()
{

    if (Owner_Car) {
        ABullet* bullet_ptr;

        Barrel_Location = Brick->GetSocketLocation("barrel");
        Barrel_Rotation = Brick->GetSocketRotation("barrel");

        bullet_ptr = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), Barrel_Location, Barrel_Rotation);
        FVector direction = Barrel_Rotation.Vector();

        if (bullet_ptr != nullptr)
            bullet_ptr->addFireImpulse(direction, 50000);
    }

}

