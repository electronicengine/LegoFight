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
#include "Components/Image.h"
#include "Slate/SlateBrushAsset.h"



bool UInventoryWidget::Initialize()
{
    Super::Initialize();
   // Button_1x1Triangle->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1TriangleClicked);


    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::General, Button_General_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Vehicle, Button_Vehicle_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Weapon, Button_Weapon_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Building, Button_Building_Category));

    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarFender4x2, Button_CarFender4x2));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarChassis1, Button_CarChassis1));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarFar1x1, Button_CarFar1x1));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarHook1x1, Button_CarHook1x1));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarHood2x2, Button_CarHood2x2));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarSeat2x2, Button_CarSeat2x2));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CarBodyWork2x1, Button_CarBodyWork2x1));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::CompleteTrack, Button_CompleteTrack));
    VehicleBrick_Buttons.push_back(std::make_pair(BrickType::Embrasure2x1, Button_Embrasure2x1));

    WeaponBrick_Buttons.push_back(std::make_pair(BrickType::Cannon2x2, Button_Cannon2x2));
    WeaponBrick_Buttons.push_back(std::make_pair(BrickType::MachineGun2x2, Button_MachineGun2x2));
    WeaponBrick_Buttons.push_back(std::make_pair(BrickType::Ax2x2, Button_Ax2x2));
    WeaponBrick_Buttons.push_back(std::make_pair(BrickType::Hammer2x2, Button_Hammer2x2));

    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low1x1, Button_Low1x1));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High1x1, Button_High1x1));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low2x1, Button_Low2x1));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High2x1, Button_High2x1));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low4x1, Button_Low4x1));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low2x2, Button_Low2x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High2x2, Button_High2x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low4x2, Button_Low4x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High4x2, Button_High4x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low4x4, Button_Low4x4));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High4x4, Button_High4x4));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low6x2, Button_Low6x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High6x2, Button_High6x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Low6x4, Button_Low6x4));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::High6x4, Button_High6x4));

    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Trapezoid2x2, Button_Trapezoid2x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Triangle2x2, Button_Triangle2x2));
    GeneralBrick_Buttons.push_back(std::make_pair(BrickType::Blend2x2, Button_Blend2x2));

    Color_Buttons.push_back(std::make_pair(BrickColor::White, Button_White));
    Color_Buttons.push_back(std::make_pair(BrickColor::Gray, Button_Gray));
    Color_Buttons.push_back(std::make_pair(BrickColor::Black, Button_Black));
    Color_Buttons.push_back(std::make_pair(BrickColor::Yellow, Button_Yellow));
    Color_Buttons.push_back(std::make_pair(BrickColor::Orange, Button_Orange));
    Color_Buttons.push_back(std::make_pair(BrickColor::Brown, Button_Brown));
    Color_Buttons.push_back(std::make_pair(BrickColor::Red, Button_Red));
    Color_Buttons.push_back(std::make_pair(BrickColor::Green, Button_Green));
    Color_Buttons.push_back(std::make_pair(BrickColor::Purple, Button_Purple));
    Color_Buttons.push_back(std::make_pair(BrickColor::Blue, Button_Blue));
    Color_Buttons.push_back(std::make_pair(BrickColor::Cyan, Button_Cyan));


    Current_Category_Buttons = &GeneralBrick_Buttons;
    setVisibilityCategoryButtons(GeneralBrick_Buttons, true);
    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
    setVisibilityCategoryButtons(Color_Buttons, false);


    Game_Instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

    return true;
}


void UInventoryWidget::removeWidget()
{
    SetVisibility(ESlateVisibility::Collapsed);

    Game_Instance->loadCharacterPanel();
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    controller->SetInputMode(FInputModeGameOnly());

}

void UInventoryWidget::checkCategory()
{



    for (int i = 0; i < Category_Buttons.size(); i++) {

        if (Category_Buttons[i].second != nullptr) {
            if (Category_Buttons[i].second->IsPressed()) {

                switch (Category_Buttons[i].first)
                {

                case BrickMenuCategory::General:
                    Current_Category_Buttons = &GeneralBrick_Buttons;
                    setVisibilityCategoryButtons(GeneralBrick_Buttons, true);
                    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
                    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
                    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
                    setVisibilityCategoryButtons(Color_Buttons, false);

                    break;

                case BrickMenuCategory::Weapon:
                    Current_Category_Buttons = &WeaponBrick_Buttons;

                    setVisibilityCategoryButtons(GeneralBrick_Buttons, false);
                    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
                    setVisibilityCategoryButtons(WeaponBrick_Buttons, true);
                    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
                    setVisibilityCategoryButtons(Color_Buttons, false);

                    break;

                case BrickMenuCategory::Vehicle:
                    Current_Category_Buttons = &VehicleBrick_Buttons;

                    setVisibilityCategoryButtons(GeneralBrick_Buttons, false);
                    setVisibilityCategoryButtons(VehicleBrick_Buttons, true);
                    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
                    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
                    setVisibilityCategoryButtons(Color_Buttons, false);

                    break;

                case BrickMenuCategory::Building:
                    Current_Category_Buttons = &BuildingBrick_Buttons;

                    setVisibilityCategoryButtons(GeneralBrick_Buttons, false);
                    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
                    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
                    setVisibilityCategoryButtons(BuildingBrick_Buttons, true);
                    setVisibilityCategoryButtons(Color_Buttons, false);

                    break;

                default:
                    break;
                }
            }
        }

    }

}

void UInventoryWidget::setVisibilityCategoryButtons(const std::vector<std::pair<int, UButton*>>& CategoryButtons, bool Visible)
{
    for (int i = 0; i < CategoryButtons.size(); i++) {
        if (Visible) {
            CategoryButtons[i].second->SetVisibility(ESlateVisibility::Visible);
        }
        else {
            CategoryButtons[i].second->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{

    if (GetVisibility() == ESlateVisibility::Visible) {

        checkCategory();
        std::vector<std::pair<int, UButton*>>& category = *Current_Category_Buttons;

        if (category == Color_Buttons) {
            for (int i = 0; i < category.size(); i++) {

                if (category[i].second != nullptr) {
                    if (category[i].second->IsPressed()) {

                        //Game_Instance->selectCurrentProductBrickColor((BrickColor)category[i].first);
                        Game_Instance->selectCurrentProductBrickColor(FLinearColor(0.4f,0.2f, 0.3f));
                        removeWidget();
                    }
                }

            }
        }
        else
        {
            for (int i = 0; i < category.size(); i++) {

                if (category[i].second != nullptr) {
                    if (category[i].second->IsPressed()) {
                        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,
                            FString::FromInt(category[i].first));

                        Game_Instance->selectCurrentProductBrick((BrickType)category[i].first);
                        
                        setVisibilityCategoryButtons(GeneralBrick_Buttons, false);
                        setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
                        setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
                        setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
                        setVisibilityCategoryButtons(Color_Buttons, true);

                        Current_Category_Buttons = &Color_Buttons;
                    }
                }

            }
        }


    }
   

}


