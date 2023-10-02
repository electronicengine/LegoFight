// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Bullet.generated.h"

UCLASS()
class LEGOFIGHT_API ABullet : public AActor
{
	GENERATED_BODY()

		bool First_Hit;
public:	
	// Sets default values for this actor's properties
	ABullet();

    UPROPERTY(EditAnywhere, Category="BulletMesh")
    UStaticMeshComponent *Bullet_Mesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    void addFireImpulse(FVector Direction, float Strenght);

private:

    // FUNCTIONS

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
};
