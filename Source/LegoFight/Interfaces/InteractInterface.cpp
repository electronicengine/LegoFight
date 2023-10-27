// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractInterface.h"
#include "../Vehicles/LegoCarChasis.h"
#include "../LegoFightGameInstance.h"
#include "../Widgets/CharacterWidget.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"
#include "Engine/StaticMesh.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Guns/Weapon.h"

// Add default functionality here for any IInteractInterface functions that are not pure virtual.


IInteractInterface::IInteractInterface()
{
    Builder_Mode_Enable = false;
}


void IInteractInterface::equip()
{
    if (Builder_Mode_Enable) {
        if (Cast<AConstraitBrick>(Target_Plugable_Item)) {
            if (!Cable_Start_Item) {
                Cable_Start_Item = Cast<AConstraitBrick>(Target_Plugable_Item);
            }
            else if(Cable_Start_Item->Cable_Hook) {
                Cable_Start_Item->hookCableToItem(Cast<ABrick>(Target_Plugable_Item)->Brick);
                Cable_Start_Item = nullptr;
            }else {
                Cast<AConstraitBrick>(Target_Plugable_Item)->turnOnOffMachine();
            }
        }
        else {
            if (Cable_Start_Item) {
                if (Cast<ABrick>(Target_Plugable_Item)) {
                    Cable_Start_Item->hookCableToItem(Cast<ABrick>(Target_Plugable_Item)->Brick);
                    Cable_Start_Item = nullptr;
                }

            }
            else {
                if (Grabbable_Brick == nullptr)
                {
                    if (Cast<ABrick>(Target_Plugable_Item)) {
                        Grabbable_Brick = Cast<ABrick>(Target_Plugable_Item);
                        grapObject(Grabbable_Brick);
                    }
                }
            }
        }

    }
    else {
        if (Cast<ALegoCarChasis>(this)) {
            exitCar();
        }
        else {

            if (Cast<ALegoCarChasis>(Object_NearBy))
            {
                enteredToCar();
                Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setInteractButtonVisibilty(ESlateVisibility::Visible);
                Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setAimButtonVisibilty(ESlateVisibility::Hidden);



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
            Cast<ALegoCharacter>(this)->AddActorLocalOffset(FVector(0,0,100));
            Cast<ALegoCharacter>(this)->SetActorHiddenInGame(true);

            Interactable_Car->enterCar(Cast<ALegoCharacter>(this));
            Is_In_Car = true;

        }
    }



}


void IInteractInterface::buyBrick()
{
    
    if (Grabbable_Brick == nullptr && Game_Instance) {
        ALegoCharacter* charc = Cast<ALegoCharacter>(this);
        AActor *spawned_object = Game_Instance->spawnItem(charc->GetActorLocation(), FRotator(0, 0, 0), "", Aim_Impact_Point);

        Grabbable_Brick = Cast<ABrick>(spawned_object);
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

    if (Game_Instance) {
            Game_Instance->savePanel(Cast<IBuiltInInterface>(Target_Plugable_Item), Aim_Impact_Point);
    }
    else {

    }

}

void IInteractInterface::interactNearby(AActor* OtherActor)
{
    ALegoCarChasis* car = Cast<ALegoCarChasis>(OtherActor);
    if (car != nullptr) {
        Object_NearBy = car;
    }
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


bool IInteractInterface::carHasPassenger()
{
    if (Passenger_ == nullptr)
        return false;
    else
        return true;
}



void IInteractInterface::enterCar(ALegoCharacter* LegoChar)
{
    //if(LegoChar != nullptr && Car_Seat != nullptr)
    //{

    Passenger_ = LegoChar;

    AController* lego_man_controller = Passenger_->GetController();

    //Passenger_->SetActorRotation(Car_Seat->GetActorRotation());

    Passenger_->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
    Passenger_->GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);


    lego_man_controller->Possess(Cast<ALegoCarChasis>(this));

    if (Weapons.size() > 0) {

        Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setFireButtonVisibilty(ESlateVisibility::Visible);

    }


    /*       Passenger_->AttachToActor(Car_Seat, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
                                                                            EAttachmentRule::KeepWorld,
                                                                            EAttachmentRule::KeepWorld, true), TEXT("seat"));*/
                                                                            //}

                                                                        //    GetMesh()->SetSimulatePhysics(true);

}



void IInteractInterface::exitCar()
{

    AController* car_controller = Cast<ALegoCarChasis>(this)->GetController();

    car_controller->Possess(Passenger_);

    const FDetachmentTransformRules& attachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
        EDetachmentRule::KeepWorld,
        EDetachmentRule::KeepWorld, true);
    Passenger_->DetachFromActor(attachment_rules);
    Passenger_->SetActorLocation(Cast<ALegoCarChasis>(this)->GetActorLocation());
    Passenger_->AddActorWorldOffset(FVector(100, 0, 10));
    Passenger_->SetActorHiddenInGame(false);

    Cast<IInteractInterface>(Passenger_)->exitedFromCar();

    Passenger_ = nullptr;

    Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setAimButtonVisibilty(ESlateVisibility::Visible);
    Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setFireButtonVisibilty(ESlateVisibility::Hidden);

    //    GetMesh()->SetSimulatePhysics(false);

}
