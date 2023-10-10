// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractInterface.h"
#include "../Vehicles/LegoCarChasis.h"
#include "../LegoFightGameInstance.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.


IInteractInterface::IInteractInterface()
{
}


void IInteractInterface::equip()
{

    if (Cast<ALegoCarChasis>(Object_NearBy))
    {
        enteredToCar();

    }
    else if (Cast<ABrick>(Object_NearBy))
    {

        if (Grabbable_Brick == Cast<ABrick>(Object_NearBy))
            dropObject(Grabbable_Brick);
        else
        {
            if (Grabbable_Brick == nullptr)
            {
                Grabbable_Brick = Cast<ABrick>(Object_NearBy);
                grapObject(Grabbable_Brick);

            }
            else
            {

                dropObject(Grabbable_Brick);
                Grabbable_Brick = Cast<ABrick>(Object_NearBy);

                grapObject(Grabbable_Brick);

            }
        }


    }
    else
    {
        if (Grabbable_Brick != nullptr)
            dropObject(Grabbable_Brick);

    }

}




void IInteractInterface::enteredToCar()
{

    if (Grabbable_Brick != nullptr)
        dropObject(Grabbable_Brick);

    if (Object_NearBy != nullptr)
    {
        Interactable_Car = Cast<ALegoCarChasis>(Object_NearBy);

        if (Interactable_Car != nullptr)
        {


            Interactable_Car->enterCar(Cast<ALegoCharacter>(this));
            Is_In_Car = true;

        }
    }



}


void IInteractInterface::buyBrick()
{
    if(Game_Instance)
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, TEXT("buy"));

    if (Grabbable_Brick == nullptr && Game_Instance) {
        Grabbable_Brick = Cast<ABrick>(Game_Instance->spawnItem(FVector(0, 0, 0), FRotator(0, 0, 0)));

        if (Grabbable_Brick != nullptr)
        {

            grapObject(Grabbable_Brick);
            Brick_Just_Plug = false;
        }
    }


}

void IInteractInterface::exitedFromCar()
{
    Is_In_Car = false;
    Interactable_Car = nullptr;
    Object_NearBy = nullptr;

    Cast<ALegoCharacter>(this)->AddActorLocalOffset(FVector(0, -250, 0));
    Cast<ALegoCharacter>(this)->GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    Cast<ALegoCharacter>(this)->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    Is_In_Car = false;
}


void IInteractInterface::openInventoryWidget()
{
    if(Game_Instance)
        Game_Instance->loadInvantoryPanel();

}

void IInteractInterface::saveAndLoad()
{

    ALegoCarChasis* vehicle = Cast<ALegoCarChasis>(Target_Plugable_Item);
    if (Game_Instance) {
        Game_Instance->savePanel(vehicle, Aim_Impact_Point);
    }
    else {

    }

}

void IInteractInterface::interactNearby(AActor* OtherActor)
{
    ALegoCarChasis* car = Cast<ALegoCarChasis>(OtherActor);
    if (car != nullptr)
        Object_NearBy = car;
    else
    {
        ABrick* brick = Cast<ABrick>(OtherActor);
        if (brick != nullptr) {

            if (brick->Owner_Car != nullptr) {
                Object_NearBy = brick->Owner_Car;
            }
            else {
                Object_NearBy = brick;
            }

        }
    }
}
