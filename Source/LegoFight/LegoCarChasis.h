// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>
#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "LegoCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CarSeat.h"
#include "Weapon.h"
#include "FireWeapon.h"
#include "MeleeWeapon.h"
#include "LegoCarChasis.generated.h"


#define BRICK_LENGHT        50

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ALegoCarChasis : public AWheeledVehicle
{
	GENERATED_BODY()
	

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<UPhysicsConstraintComponent*> Plugin_Sockets;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<USceneComponent *> Plugin_Points;

    ALegoCharacter *Passenger_;

    uint8 Socket_Number_Width;
    uint8 Socket_Number_Height;
    uint8 Socket_Number;

    uint8 Plugin_Number_Width;
    uint8 Plugin_Number_Height;
    uint8 Plugin_Number;

    ACarSeat *Car_Seat;

    TArray<AFireWeapon *> Armed_Fire_Weapons;
    TArray<AMeleeWeapon *> Armed_Melee_Weapons;

public:

    ALegoCarChasis();

    virtual void BeginPlay() override;
	
    void enterCar(ALegoCharacter *LegoChar);
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void moveForward(float Value);
    void moveRight(float Value);
    void turn(float Value);
    void lookUp(float Value);

    void fire();
    void equip();
    void exitCar();

    float calculateDistance(const FVector &Vector1, const FVector &Vector2);
    int getClosestPluginIndex(const std::vector<float> &Array);
    UPhysicsConstraintComponent *findClosestContrait(const FVector &Location);


    int Current_Plugin_Index;

    UPROPERTY(EditAnywhere)
    UPhysicsConstraintComponent* Current_Closest_Constrait;

    FVector getPlugin(const FVector &ImpactPoint);
    FRotator getPluginRotation();
    void addWeaponToInventory(AWeapon *Weapon);


    void plugTheBrick(ABrick *Object, const FRotator &OffsetRotation);
    void highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
                         ABrick *Interactable_Brick, const FHitResult &OutHit, const FRotator &OffetRotation);
};
