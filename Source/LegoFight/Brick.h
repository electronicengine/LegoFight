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

#include "Brick.generated.h"

#define BRICK_LENGHT        50



class ALegoCarChasis;

enum BrickType
{
    Lego_Hammer,
    Lego_Ax,
    Lego_Machine_Gun,
    Lego_Cannon,
    Lego_Physics_Weapon,
    Lego_Melee_Weapon,
    Lego_Fire_Weapon,
    Lego_Car_Seat,
    Lego1x1_Comp,
    Lego1x1_Semi,
    Lego1x1_Even,
    Lego1x1_Trapezoid,
    Lego1x1_Triangle,
    Lego2x1_Comp,
    Lego2x1_Semi,
    Lego2x2_Comp,
    Lego2x2_Semi,
    Lego3x1_Comp,
    Lego3x1_Semi,
    Lego3x2_Comp,
    Lego3x2_Semi,

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


UCLASS()
class LEGOFIGHT_API ABrick : public AActor, public IBuiltInInterface, public IPlugInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent *Brick;

    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category="Bullet")
    TSubclassOf<class ADestrictable> Destructible_Container;


    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh *Brick_Mesh;


    UPROPERTY(EditAnywhere)
    UBoxComponent *Collision_Box;

    int Healt_;

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    virtual USceneComponent *CreatePluginPoint(FString Name) override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void enablePhysics(bool Value);
    UStaticMesh *getBrickMesh();
    void addDamage(int Value);
    void breakBrick();

    BrickType Type_;
    BrickSubType Sub_Type;


    std::vector<float> Distance_Array;
    void setLegoCarOwner(ALegoCarChasis *Car);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


    bool First_Hit;

};
