// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <iostream>
#include <vector>
#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CarSeat.h"
#include "../Guns/Weapon.h"
#include "../Characters/LegoCharacter.h"
#include "../Brick.h"
#include "../Interfaces/BuiltInInterface.h"
#include "../Interfaces/PlugInterface.h"
#include "LegoCarChasis.generated.h"


#define BRICK_LENGHT        50

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ALegoCarChasis : public AWheeledVehicle, public IBuiltInInterface, public IPlugInterface
{
	GENERATED_BODY()
	

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;

    UPROPERTY(EditAnywhere)


    ALegoCharacter *Passenger_;

    ACarSeat *Car_Seat;

    TArray<AWeapon *> Weapons;

    bool Weapon_Camera_Used;
    int Current_Camera_Index;

    void setupWheels();
    void setupMaterials();

protected:
    virtual USceneComponent *CreatePluginPoint(FString Name) override;


public:

    ALegoCarChasis();

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


    bool carHasPassenger();

    void enterCar(ALegoCharacter *LegoChar);

    void moveForward(float Value);
    void moveRight(float Value);
    void turn(float Value);
    void lookUp(float Value);
    void interact();

    void fire();
    void equip();
    void exitCar();

    void addWeaponToInventory(AWeapon *Weapon);
    void addSeatToCar(ACarSeat *Seat);



    UPROPERTY(EditAnywhere, Category="Team")
    int Team_Id;

    int Current_Plugin_Index;

    UPROPERTY(EditAnywhere)
    UPhysicsConstraintComponent* Current_Closest_Constrait;
};
