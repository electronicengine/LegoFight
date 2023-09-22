// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Brick.h"
#include "../Guns/Bullet.h"
#include "Materials/Material.h"
#include "LegoCharacter.generated.h"


class ALegoCarChasis;
class AWeapon;

UCLASS()
class LEGOFIGHT_API ALegoCharacter : public ACharacter
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Aim_Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent* Interaction_Component;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent *Ghost_Component;

    UPROPERTY(EditAnywhere)
    UMaterial *Ghost_Possible_Material;

    UPROPERTY(EditAnywhere)
    UMaterial *Ghost_Imposible_Material;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category="Bullet")
    TSubclassOf<class ABullet> Bullet_Container;

    UPROPERTY(EditAnywhere, Category="Barrel")
    USceneComponent *Barrel_;

    ABrick *Ghost_Overlapped_Brick;
    bool Brick_Just_Plug;
    bool Brick_Plugable;
    FVector Aim_Impact_Point;

    AActor *Object_NearBy;

    ABrick *Grabbable_Brick;
    AWeapon *Interactable_Weapon;
    ALegoCarChasis *Interactable_Car;

    ALegoCarChasis *Target_Car;
    ABrick *Target_Brick;
    int Tick_Count;

    FRotator OffSet_Rotation;

    void setupMesh();

public:
	// Sets default values for this character's properties
	ALegoCharacter();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Camera;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool Is_In_Car;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool Is_In_Use_Weapon;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool Keeping_Bricks;

    bool Aiming_;

    UFUNCTION()
    void OnDelegateOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnGhostOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    void moveForward(float Value);
    void moveRight(float Value);
    void turn(float Value);
    void lookUp(float Value);

    void fire();
    void aimStart();
    void aimEnd();
    void equip();
    void interact();
    void turnObject();

    void grapObject(ABrick *Object);
    void plugObject();
    void dropObject(ABrick *Object);

    void enteredToCar();
    void exitedFromCar();
    void useWeapon();
    void openInventoryWidget();
    void buyBrick();
    void save();
    void load();


    ConstructionInfo Construction_Info;
	
};
