// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



ULegoFightGameInstance::ULegoFightGameInstance()
{

    Selected_Brick = Lego1x1_Comp;
    Invantory_Panel = nullptr;
    Char_Panel = nullptr;
}



UUserWidget *ULegoFightGameInstance::loadInvantoryPanel()
{

    if(Invantory_Panel == nullptr)
    {
        if(Char_Panel != nullptr)
        {
            Char_Panel->RemoveFromParent();
            Char_Panel = nullptr;
        }

        Invantory_Panel = CreateWidget<UUserWidget>(this, Invantory_Panel_Container);

        Invantory_Panel->AddToViewport();

        APlayerController *controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        controller->bShowMouseCursor = true;
    //    controller->bEnableClickEvents = true;
    //    controller->bEnableMouseOverEvents = true;
    }



    return Invantory_Panel;
}



UUserWidget *ULegoFightGameInstance::loadCharacterPanel()
{

    if(Char_Panel == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, -5 ,FColor::Orange, TEXT("loadCharacterPanel"));

        if(Invantory_Panel != nullptr)
        {
            Invantory_Panel->RemoveFromParent();
            Invantory_Panel = nullptr;
        }

        Char_Panel = CreateWidget<UUserWidget>(this, Character_Panel_Container);

        if(Char_Panel != nullptr)
            Char_Panel->AddToViewport();

        APlayerController *controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        controller->bShowMouseCursor = false;
    //    controller->bEnableClickEvents = false;
    //    controller->bEnableMouseOverEvents = false;

    }

    return Char_Panel;
}



ABrick *ULegoFightGameInstance::spawnBrick(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{

    ABrick *spawn_brick;
    int type;

    if(Type < 0)
        type = Selected_Brick;
    else
        type = Type;

    switch (type)
    {
        case Lego1x1_Trapezoid:

        spawn_brick = GetWorld()->SpawnActor<ALego1x1Trapezoid>(Lego1x1Trapezoid_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego1x1_Triangle:

        spawn_brick = GetWorld()->SpawnActor<ALego1x1Triangle>(Lego1x1Triangle_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego1x1_Comp:

        spawn_brick = GetWorld()->SpawnActor<ALego1x1Comp>(Lego1x1Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego2x1_Comp:

        spawn_brick = GetWorld()->SpawnActor<ALego2x1Comp>(Lego2x1Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego2x2_Comp:

        spawn_brick = GetWorld()->SpawnActor<ALego2x2Comp>(Lego2x2Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego3x1_Comp:

        spawn_brick = GetWorld()->SpawnActor<ALego3x1Comp>(Lego3x1Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego3x2_Comp:

        spawn_brick = GetWorld()->SpawnActor<ALego3x2Comp>(Lego3x2Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego1x1_Semi:

        spawn_brick = GetWorld()->SpawnActor<ALego1x1Semi>(Lego1x1Semi_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego2x1_Semi:

        spawn_brick = GetWorld()->SpawnActor<ALego2x1Semi>(Lego2x1Semi_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego2x2_Semi:

        spawn_brick = GetWorld()->SpawnActor<ALego2x2Semi>(Lego2x2Semi_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego3x1_Semi:

        spawn_brick = GetWorld()->SpawnActor<ALego3x1Semi>(Lego3x1Semi_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego3x2_Semi:

        spawn_brick = GetWorld()->SpawnActor<ALego3x2Semi>(Lego3x2Semi_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego_Cannon:

        spawn_brick = GetWorld()->SpawnActor<AFireWeapon>(Cannon_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        case Lego_Machine_Gun:

        spawn_brick = GetWorld()->SpawnActor<AFireWeapon>(Machine_Gun_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;
        case Lego_Ax:

        spawn_brick = GetWorld()->SpawnActor<AMeleeWeapon>(Ax_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;
        case Lego_Hammer:

        spawn_brick = GetWorld()->SpawnActor<AMeleeWeapon>(Hammer_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;

        default:

        spawn_brick = GetWorld()->SpawnActor<ALego1x1Comp>(Lego1x1Comp_Container, SpawnLocation, SpawnRotation);
            return spawn_brick;
    }

}


ALegoCarChasis *ULegoFightGameInstance::spawnVehicle(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{
    ALegoCarChasis *vehicle;

    switch (Type)
    {
        case Lego_Enemy_Vehicle:

        vehicle = GetWorld()->SpawnActor<AEnemyLegoVehicle>(Lego_Enemy_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;

        case Lego_Char_Vehicle:

        vehicle = GetWorld()->SpawnActor<ALegoCarChasis>(Lego_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;

        default:

        vehicle = GetWorld()->SpawnActor<AEnemyLegoVehicle>(Lego_Enemy_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;
    }
}



void ULegoFightGameInstance::selectCurrentProductBrick(BrickType Type)
{
    Selected_Brick = Type;
}
