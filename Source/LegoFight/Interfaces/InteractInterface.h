// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuilderInterface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UBuilderInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOFIGHT_API IInteractInterface : public IBuilderInterface
{
	GENERATED_BODY()

protected:

    ALegoCarChasis* Interactable_Car;
    class ULegoFightGameInstance* Game_Instance;

    bool Is_In_Car;

public:
    IInteractInterface();
    void equip();


    void buyBrick();

    void enteredToCar();
    void exitedFromCar();
    void openInventoryWidget();
    void saveAndLoad();
    void interactNearby(AActor* OtherActor);
};
