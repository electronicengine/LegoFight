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
#include "Camera/CameraComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PathFinder.h"
#include "LegoCarChasis.h"
#include "EnemyLegoVehicle.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AEnemyLegoVehicle : public ALegoCarChasis
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<APathFinder> Path_Finder_Container;

    APathFinder *Path_Finder;

    UPROPERTY(EditAnywhere)
    UBoxComponent *Overlap_Box;

    void followThePathFinder(const FVector& Location);

public:
    AEnemyLegoVehicle();
    bool Crash_Target;

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void moveLocation(const FVector &Location);

    UFUNCTION()
    void OnDelegateOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

    FVector Destination_Location;

};
