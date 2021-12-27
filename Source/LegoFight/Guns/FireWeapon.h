// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Engine/Engine.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "FireWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AFireWeapon : public AWeapon
{
	GENERATED_BODY()
	

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category="Bullet")
    TSubclassOf<class ABullet> Bullet_Container;

    UPROPERTY(EditAnywhere, Category="Barrel")
    USceneComponent *Barrel_;



public:
    AFireWeapon();

    void fire();
    FVector getBarrelVector();
	
};
