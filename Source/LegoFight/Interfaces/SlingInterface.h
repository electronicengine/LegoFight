// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SlingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USlingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOFIGHT_API ISlingInterface
{
	GENERATED_BODY()

protected:
	UGameViewportClient* Viewport_Client;
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	ISlingInterface();

	bool Sling_Mode_Enable;
	bool Stretching_Sling;

	FVector Stretch_Begin;
	FVector Stretch_End;
	FVector Throw_Force; 

	void calculateProjectilePath(AActor* Projectile);
	void throwTheItem(const FVector& ForceVector, AActor* Item);
	void strechTheSling();
	AActor* Projectile_Item;


};
