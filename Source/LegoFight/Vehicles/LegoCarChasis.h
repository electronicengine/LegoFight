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
#include "../Interfaces/InteractInterface.h"

#include "LegoCarChasis.generated.h"


#define BRICK_LENGHT        50

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ALegoCarChasis : public AWheeledVehicle, public IBuiltInInterface, public IPlugInterface, public IInteractInterface
{
	GENERATED_BODY()
	

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;


    void setupWheels();
    void setupMaterials();

protected:
    virtual USceneComponent *CreatePluginPoint(FString Name) override;


    ACarSeat* Car_Seat;

    int Current_Weapon_Index;

    bool Weapon_Camera_Used;
    int Current_Camera_Index;
    float Healt;

public:

    ALegoCarChasis();

    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



    void moveForward(float Value);
    void moveRight(float Value);
    void turn(float Value);
    void lookUp(float Value);
    void interact();

    void fire();
  
    void jump();

    int removeWeaponToInventory(int Index);
    int addWeaponToInventory(AWeapon *Weapon);
    void addSeatToCar(ACarSeat *Seat);
    void addDamage(float Value);
    void breakBrick();


    UPROPERTY(EditAnywhere, Category="Team")
    int Team_Id;

    int Current_Plugin_Index;

    UPROPERTY(EditAnywhere)
    UPhysicsConstraintComponent* Current_Closest_Constrait;
};
