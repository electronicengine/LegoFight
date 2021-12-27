// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Engine/Engine.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"
#include "../LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RendererInterface.h"
#include "DrawDebugHelpers.h"
#include "../LegoFightGameInstance.h"
#include "Engine/World.h"



bool UInventoryWidget::Initialize()
{
    Super::Initialize();

    Button_CannonComp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_CannonCompClicked);
    Button_MachineGunComp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_MachineGunCompClicked);
    Button_AxComp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_AxCompClicked);
    Button_HammerComp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_HammerCompClicked);
    Button_1x1Comp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1CompClicked);
    Button_2x1Comp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_2x1CompClicked);
    Button_2x2Comp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_2x2CompClicked);
    Button_3x1Comp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_3x1CompClicked);
    Button_3x2Comp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_3x2CompClicked);
    Button_1x1Semi->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1SemiClicked);
    Button_2x1Semi->OnClicked.AddDynamic(this, &UInventoryWidget::Button_2x1SemiClicked);
    Button_2x2Semi->OnClicked.AddDynamic(this, &UInventoryWidget::Button_2x2SemiClicked);
    Button_3x1Semi->OnClicked.AddDynamic(this, &UInventoryWidget::Button_3x1SemiClicked);
    Button_3x2Semi->OnClicked.AddDynamic(this, &UInventoryWidget::Button_3x2SemiClicked);
    Button_1x1Trapezoid->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1TrapezoidClicked);
    Button_1x1Triangle->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1TriangleClicked);

    Button_CannonComp->OnClicked.AddDynamic(this, &UInventoryWidget::Button_CannonCompClicked);

    return true;
}

void UInventoryWidget::Button_1x1TrapezoidClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego1x1_Trapezoid);

    removeWidget();

}

void UInventoryWidget::Button_1x1TriangleClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego1x1_Triangle);

    removeWidget();

}

void UInventoryWidget::Button_1x1CompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego1x1_Comp);

    removeWidget();

}

void UInventoryWidget::Button_2x1CompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego2x1_Comp);

    removeWidget();
}

void UInventoryWidget::Button_2x2CompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego2x2_Comp);

    removeWidget();
}

void UInventoryWidget::Button_3x1CompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego3x1_Comp);

    removeWidget();
}

void UInventoryWidget::Button_3x2CompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego3x2_Comp);

    removeWidget();
}

void UInventoryWidget::Button_1x1SemiClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego1x1_Semi);

    removeWidget();
}

void UInventoryWidget::Button_2x1SemiClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego2x1_Semi);

    removeWidget();
}

void UInventoryWidget::Button_2x2SemiClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego2x2_Semi);

    removeWidget();
}

void UInventoryWidget::Button_3x1SemiClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego3x1_Semi);

    removeWidget();
}

void UInventoryWidget::Button_3x2SemiClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego3x2_Semi);

    removeWidget();
}

void UInventoryWidget::Button_CannonCompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego_Cannon);

    removeWidget();
}

void UInventoryWidget::Button_MachineGunCompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego_Machine_Gun);

    removeWidget();
}

void UInventoryWidget::Button_HammerCompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego_Hammer);

    removeWidget();
}

void UInventoryWidget::Button_AxCompClicked()
{
    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->selectCurrentProductBrick(Lego_Ax);

    removeWidget();
}

void UInventoryWidget::removeWidget()
{
    this->RemoveFromParent();

    ULegoFightGameInstance *instance = Cast<ULegoFightGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    instance->loadCharacterPanel();
}

