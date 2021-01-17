// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.h"
#include "Lego1x1Triangle.generated.h"

UCLASS()
class LEGOFIGHT_API ALego1x1Triangle : public ABrick
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALego1x1Triangle();


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
