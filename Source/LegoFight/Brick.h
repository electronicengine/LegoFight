// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Destrictable.h"
#include "Interfaces/BuiltInInterface.h"
#include "Interfaces/PlugInterface.h"
#include <vector>
#include <map>
#include <queue>

#include "Brick.generated.h"

#define BRICK_LENGHT        50



class ALegoCarChasis;
enum BrickColor {
    White = 1000,
    Gray,
    Black,
    Yellow,
    Orange,
    Green,
    Brown,
    Red,
    Purple,
    Blue,
    Cyan

};



enum ItemType
{
    Low1x1,
    High1x1,
    Low2x1,
    High2x1,
    Low4x1,
    Low2x2,
    High2x2,
    Low4x2,
    High4x2,
    Low4x4,
    High4x4,
    Low6x2,
    High6x2,
    Low6x4,
    High6x4,
    Trapezoid2x2,
    Triangle2x2,
    Blend2x2,

    CarHook1x1,
    CarFar1x1,
    CarFender4x2,
    CarBodyWork2x1,
    CarHood2x2,
    Embrasure2x1,
    CarSeat2x2,

    CarChassis1 = 100,
    CompleteTrack,

    Hammer2x2 = 200,
    Ax2x2,
    MachineGun2x2,
    Cannon2x2,

};


enum BrickSubType
{
    Comp,
    Semi
};


enum VehicleType
{
    Lego_Char_Vehicle = 30,
    Lego_Enemy_Vehicle
};


struct ItemOptions;

UCLASS()
class LEGOFIGHT_API ABrick : public AActor, public IBuiltInInterface, public IPlugInterface
{
    GENERATED_BODY()

        FVector Brick_Color;

public:
    // Sets default values for this actor's properties
    ABrick();

    std::queue<USceneComponent*> plugs;

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* Brick;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = "Bullet")
        TSubclassOf<class ADestrictable> Destructible_Container;


    UPROPERTY(BlueprintReadWrite, EditAnywhere)
        UStaticMesh* Brick_Mesh;


    UPROPERTY(EditAnywhere)
        UMaterial* Material;

    UPROPERTY(EditAnywhere)
        USceneComponent* Default_Root;
    int Healt_;


protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual USceneComponent *CreatePluginPoint(FString Name) override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void enablePhysics(bool Value);
    void setCollisionProfile(FString Profile);

    UStaticMesh *getBrickMesh();
    void addDamage(int Value);
    void breakBrick();
    void setMaterialColor(FLinearColor Color);
    void setBrickTypeOptions(ItemOptions& Options);

    ItemType Type_;
    BrickSubType Sub_Type;


    std::vector<float> Distance_Array;
    void setLegoCarOwner(ALegoCarChasis *Car);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    UStaticMesh* test_mesh;
    bool First_Hit;

};
