// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Brick.generated.h"

#define BRICK_LENGHT        50


class ALegoCarChasis;

enum BrickType
{
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
    Lego3x1_semi,
    Lego3x2_Comp,
    Lego3x2_Semi

};


enum BrickSubType
{
    Comp,
    Semi
};


UCLASS()
class LEGOFIGHT_API ABrick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent *Brick;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<UPhysicsConstraintComponent*> Plugin_Sockets;

    UPROPERTY(EditAnywhere)
    UPhysicsConstraintComponent* Current_Closest_Constrait;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh *Brick_Mesh;

    UPROPERTY(EditAnywhere, Instanced)
    TArray<USceneComponent *> Plugin_Points;

    UPROPERTY(EditAnywhere)
    UBoxComponent *Collision_Box;

    ALegoCarChasis *Owner_Car;

protected:

    uint8 Socket_Number_Width;
    uint8 Socket_Number_Height;
    uint8 Socket_Number;

    uint8 Plugin_Number_Width;
    uint8 Plugin_Number_Height;
    uint8 Plugin_Number;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


    float calculateDistance(const FVector &Vector1, const FVector &Vector2);
    int getClosestPluginIndex(const std::vector<float> &Array);
    UPhysicsConstraintComponent *findClosestContrait(const FVector &Location);




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void enablePhysics(bool Value);
    UStaticMesh *getBrickMesh();
    FRotator getPluginRotation();



    BrickType Type_;
    BrickSubType Sub_Type;
    int Current_Plugin_Index;

    std::vector<float> Distance_Array;
    void setLegoCarOwner(ALegoCarChasis *Car);
    FVector getPlugin(const FVector &ImpactPoint);
    void plugTheBrick(ABrick *Object, const FRotator &OffsetRotation);
    void highLightPlugin(UStaticMeshComponent *Ghost_Brick, UMaterial *Possible_Material, UMaterial *ImPossible_Material,
                         ABrick *Interactable_Brick, const FHitResult &OutHit, const FRotator &OffetRotation);
};
