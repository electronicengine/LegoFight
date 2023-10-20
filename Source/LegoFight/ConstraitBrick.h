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

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UPhysicsConstraintComponent* Constrait;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UStaticMeshComponent* Additional_Brick;
};
