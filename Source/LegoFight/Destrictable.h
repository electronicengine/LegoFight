// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <map>
#include "EngineUtils.h"
#include <string>
#include "Destrictable.generated.h"

UCLASS()
class LEGOFIGHT_API ADestrictable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USceneComponent* Default_Root;

public:	
	// Sets default values for this actor's properties
	ADestrictable();
	UMaterial* Material;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	TArray<UStaticMeshComponent*> Pieces;
	std::map<int, std::string> Mesh_Paths;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void setColor(FVector ColorVec);
	void setMesh(FString Name);
	void setImpactPointAndVelocity(const FVector &ImpactPoint, const FVector& Velocity);

};
