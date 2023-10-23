// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Brick.h"
#include "ConstraitBrick.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AConstraitBrick : public ABrick
{
	GENERATED_BODY()



public:
	AConstraitBrick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Add;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Machine_Running;

	bool turnOnOffMachine();
	void setGhostComponent(UStaticMesh* Mesh);
	virtual void BeginPlay() override;
	virtual void setMaterialColor(FLinearColor Color);

};
