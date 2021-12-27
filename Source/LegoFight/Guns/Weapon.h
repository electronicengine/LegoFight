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


     ACharacter *User_;
public:

    AWeapon();

    void useWeapon();
	
	
};
