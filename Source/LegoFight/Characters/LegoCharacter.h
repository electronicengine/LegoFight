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
#include "../Interfaces/BuilderInterface.h"
#include "../Interfaces/InteractInterface.h"
#include "../Interfaces/SlingInterface.h"

#include "LegoCharacter.generated.h"


enum CameraMode {
    normal,
    builder,
    sling
};

class ALegoCarChasis;
class AWeapon;

UCLASS()
class LEGOFIGHT_API ALegoCharacter : public ACharacter, public IInteractInterface, public ISlingInterface
{
	GENERATED_BODY()


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* SpringArm;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Builder_Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* Sling_Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent* Interaction_Component;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Ghost_Item;


    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category="Bullet")
    TSubclassOf<class ABullet> Bullet_Container;

    UPROPERTY(EditAnywhere, Category="Barrel")
    USceneComponent *Barrel_;

    AWeapon *Interactable_Weapon;

    CameraMode Camera_Mode;


    void setupMesh();

public:
	// Sets default values for this character's properties
	ALegoCharacter();

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool Is_In_Car;

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


    UFUNCTION()
    void OnInteractBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    UFUNCTION()
    void OnInteractEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnGhostOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location);

    UFUNCTION()
    void OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location);

    UFUNCTION()
    void TouchMoved(ETouchIndex::Type FingerIndex, FVector Location);

    

    void moveForward(float Value);
    void moveRight(float Value);
    void turn(float Value);
    void lookUp(float Value);
    void zoom(float Value);
    void fire();
    void changeCameraMode();
    void aimEnd();
    void calculateProjectilePath(AActor* Projectile, const FVector& ForwardVector);


    ConstructionInfo Construction_Info;
	
};
