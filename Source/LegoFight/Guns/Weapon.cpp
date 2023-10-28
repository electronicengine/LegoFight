// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"
#include "Kismet/GameplayStatics.h"
#include "../LegoFightGameInstance.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(Brick);
    static ConstructorHelpers::FObjectFinder<UParticleSystem> particle(TEXT("/Game/effects/effect"));
    Particle_Effect = particle.Object;


    Healt_ = 900000;
    Type_ = WeaponType::fire;


}

void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (checkWeaponDetached())
        return;

    if (Owner_Car) {
        if (Owner_Car->carHasPassenger() && Type_ == WeaponType::fire)
        {

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

void AWeapon::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
 
    int crash_speed = Brick->GetPhysicsLinearVelocity().Size();

    if (Cast<ABrick>(OtherActor)) {

        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_Effect, GetActorLocation());
        Cast<ABrick>(OtherActor)->addDamage(crash_speed * 4);
    }
    else if (Cast<ALegoCarChasis>(OtherActor)) {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_Effect, GetActorLocation());
        Cast<ALegoCarChasis>(OtherActor)->addDamage(crash_speed * 4);

    }
    

}

void AWeapon::makePluginSettings()
{
    if (Brick_Name.Find(MELEE_WEAPON_APPENDIX) >= 0)
        Type_ = WeaponType::melee;
    else if (Brick_Name.Find(FIRE_WEAPON_APPENDIX) >= 0)
        Type_ = WeaponType::fire;
    else if (Brick_Name.Find(EXPLOSIVE_WEAPON_APPENDIX) >= 0)
        Type_ = WeaponType::explosive;

    switch (Type_)
    {
    case WeaponType::fire:
        if (Owner_Item || Owner_Car) {
            enablePhysics(false);
            setCollisionProfile("OverlapAll");
        }
        break;
    case WeaponType::melee:
        enablePhysics(true);
        setCollisionProfile("BlockAll");
        Brick->SetMassOverrideInKg(NAME_None, 10, true);
        Brick->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
        Brick->SetNotifyRigidBodyCollision(true);

        Brick->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
        break;
    case WeaponType::explosive:
        enablePhysics(true);
        setCollisionProfile("BlockAll");
        Brick->SetMassOverrideInKg(NAME_None, 10, true);
        Brick->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
        Brick->SetNotifyRigidBodyCollision(true);
        SetLifeSpan(20);
        Mass_ = 17;
        Brick->SetMassOverrideInKg(NAME_None, 17);
        Brick->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
        break;
    default:
        break;
    }

}


void AWeapon::useWeapon()
{



    APlayerController* controller = UGameplayStatics::GetPlayerController(this, 0);
    controller->SetViewTargetWithBlend(this);

//        controller->Possess(Cast<APawn>(this));



}

void AWeapon::fire()
{

    if (Owner_Car && Type_== WeaponType::fire) {
        ABullet* bullet_ptr;

        Barrel_Location = Brick->GetSocketLocation("barrel");
        Barrel_Rotation = Brick->GetSocketRotation("barrel");

        bullet_ptr = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), Barrel_Location, Barrel_Rotation);
        FVector direction = Barrel_Rotation.Vector();

        if (bullet_ptr != nullptr)
            bullet_ptr->addFireImpulse(direction, 50000);
    }

}

