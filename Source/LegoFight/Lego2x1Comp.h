// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.h"

#include "Lego2x1Comp.generated.h"

UCLASS()
class LEGOFIGHT_API ALego2x1Comp : public ABrick
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALego2x1Comp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

};
