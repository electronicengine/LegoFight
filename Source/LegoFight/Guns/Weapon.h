// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Brick.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Weapon.generated.h"



enum WeaponType {
    fire,
    melee,
    explosive
};

class ALegoCharacter;
class ACharacter;
/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AWeapon : public ABrick
{
	GENERATED_BODY()
	

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
        UCameraComponent* Camera;
    UParticleSystem* Particle_Effect;

    FVector Barrel_Location;
    FRotator Barrel_Rotation;
    USceneComponent* barrel;
    ACharacter *User_;
    WeaponType Type_;
    int Last_Time;

public:

    AWeapon();

    void useWeapon();
	
    void fire();

    void aimToRotation(FRotator &Rotation);
    void removeOwner();
    bool checkWeaponDetached();
    void makePluginSettings();

    virtual void Tick(float DeltaTime) override;
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	
};
