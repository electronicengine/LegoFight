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
#include "Styling/SlateBrush.h"
#include "SlateBasics.h"
#include "Widgets/Images/SImage.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"

#include "../Brick.h"



bool UInventoryWidget::Initialize()
{
    Super::Initialize();
   // Button_1x1Triangle->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1TriangleClicked);

    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::General, Button_General_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Vehicle, Button_Vehicle_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Weapon, Button_Weapon_Category));
    Category_Buttons.push_back(std::make_pair(BrickMenuCategory::Building, Button_Building_Category));

    UButton* btn;
    UTexture2D* btn_texture;
    FString name_appendix;
    TArray<UObject*> general_brick_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/widgets/thumbnails/general/"), general_brick_assets, EngineUtils::ATL_Regular);
    int i = 0;
    for (auto asset : general_brick_assets)
    {
        btn_texture = Cast<UTexture2D>(asset);
        name_appendix = FString::FromInt(i++);

        btn = NewObject<UButton>(UButton::StaticClass());
        btn->AppendName(name_appendix);

        // Set the button's background image using the slate brush
        btn->WidgetStyle.Normal.SetResourceObject(btn_texture);
        btn->WidgetStyle.Normal.SetImageSize(FVector2D(130, 130));

        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetImageSize(FVector2D(130, 130));
        btn->WidgetStyle.Hovered.TintColor = FLinearColor(0.270498, 0.270498, 0.270498);

        btn->WidgetStyle.Pressed.SetResourceObject(btn_texture);
        btn->WidgetStyle.Pressed.SetImageSize(FVector2D(130, 130));

        box->AddChild(btn);
        GeneralBrick_Buttons.push_back(std::make_pair(asset->GetName(), btn));
    }

    TArray<UObject*> vehicle_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/widgets/thumbnails/vehicles/"), vehicle_assets, EngineUtils::ATL_Regular);
    for (auto asset : vehicle_assets)
    {
        btn_texture = Cast<UTexture2D>(asset);
        name_appendix = FString::FromInt(i++);

        btn = NewObject<UButton>(UButton::StaticClass());
        btn->AppendName(name_appendix);

        // Set the button's background image using the slate brush
        btn->WidgetStyle.Normal.SetResourceObject(btn_texture);
        btn->WidgetStyle.Normal.SetImageSize(FVector2D(130, 130));

        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetImageSize(FVector2D(130, 130));
        btn->WidgetStyle.Hovered.TintColor = FLinearColor(0.270498, 0.270498, 0.270498);

        btn->WidgetStyle.Pressed.SetResourceObject(btn_texture);
        btn->WidgetStyle.Pressed.SetImageSize(FVector2D(130, 130));

        box->AddChild(btn);
        VehicleBrick_Buttons.push_back(std::make_pair(asset->GetName(), btn));
    }

    TArray<UObject*> weapon_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/widgets/thumbnails/weapons/"), weapon_assets, EngineUtils::ATL_Regular);
    for (auto asset : weapon_assets)
    {
        btn_texture = Cast<UTexture2D>(asset);
        name_appendix = FString::FromInt(i++);

        btn = NewObject<UButton>(UButton::StaticClass());
        btn->AppendName(name_appendix);

        // Set the button's background image using the slate brush
        btn->WidgetStyle.Normal.SetResourceObject(btn_texture);
        btn->WidgetStyle.Normal.SetImageSize(FVector2D(130, 130));

        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetImageSize(FVector2D(130, 130));
        btn->WidgetStyle.Hovered.TintColor = FLinearColor(0.270498, 0.270498, 0.270498);

        btn->WidgetStyle.Pressed.SetResourceObject(btn_texture);
        btn->WidgetStyle.Pressed.SetImageSize(FVector2D(130, 130));

        box->AddChild(btn);
        WeaponBrick_Buttons.push_back(std::make_pair(asset->GetName(), btn));
    }

    TArray<UObject*> buildings_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/widgets/thumbnails/buildings/"), buildings_assets, EngineUtils::ATL_Regular);
    for (auto asset : buildings_assets)
    {
        btn_texture = Cast<UTexture2D>(asset);
        name_appendix = FString::FromInt(i++);

        btn = NewObject<UButton>(UButton::StaticClass());
        btn->AppendName(name_appendix);

        // Set the button's background image using the slate brush
        btn->WidgetStyle.Normal.SetResourceObject(btn_texture);
        btn->WidgetStyle.Normal.SetImageSize(FVector2D(130, 130));

        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetImageSize(FVector2D(130, 130));
        btn->WidgetStyle.Hovered.TintColor = FLinearColor(0.270498, 0.270498, 0.270498);

        btn->WidgetStyle.Pressed.SetResourceObject(btn_texture);
        btn->WidgetStyle.Pressed.SetImageSize(FVector2D(130, 130));

        box->AddChild(btn);
        BuildingBrick_Buttons.push_back(std::make_pair(asset->GetName(), btn));
    }


    TArray<UObject*> color_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/widgets/thumbnails/colors/"), color_assets, EngineUtils::ATL_Regular);
    for (auto asset : color_assets)
    {
        btn_texture = Cast<UTexture2D>(asset);
        name_appendix = FString::FromInt(i++);

        btn = NewObject<UButton>(UButton::StaticClass());
        btn->AppendName(name_appendix);

        // Set the button's background image using the slate brush
        btn->WidgetStyle.Normal.SetResourceObject(btn_texture);
        btn->WidgetStyle.Normal.SetImageSize(FVector2D(130, 130));

        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetResourceObject(btn_texture);
        btn->WidgetStyle.Hovered.SetImageSize(FVector2D(130, 130));
        btn->WidgetStyle.Hovered.TintColor = FLinearColor(0.270498, 0.270498, 0.270498);

        btn->WidgetStyle.Pressed.SetResourceObject(btn_texture);
        btn->WidgetStyle.Pressed.SetImageSize(FVector2D(130, 130));

        box->AddChild(btn);
        Color_Buttons.push_back(std::make_pair(asset->GetName(), btn));
    }
  

    Game_Instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

    Current_Category_Buttons = &GeneralBrick_Buttons;
    setVisibilityCategoryButtons(GeneralBrick_Buttons, true);
    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
    setVisibilityCategoryButtons(Color_Buttons, false);

    return true;
}


void UInventoryWidget::removeWidget()
{
    SetVisibility(ESlateVisibility::Collapsed);

    Game_Instance->loadCharacterPanel();
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    Current_Category_Buttons = &GeneralBrick_Buttons;
    setVisibilityCategoryButtons(GeneralBrick_Buttons, true);
    setVisibilityCategoryButtons(VehicleBrick_Buttons, false);
    setVisibilityCategoryButtons(WeaponBrick_Buttons, false);
    setVisibilityCategoryButtons(BuildingBrick_Buttons, false);
    setVisibilityCategoryButtons(Color_Buttons, false);

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

void UInventoryWidget::setVisibilityCategoryButtons(const std::vector<std::pair<FString, UButton*>>& CategoryButtons, bool Visible)
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
        std::vector<std::pair<FString, UButton*>>& category = *Current_Category_Buttons;

        if (category == Color_Buttons) {
            for (int i = 0; i < category.size(); i++) {

                if (category[i].second != nullptr) {
                    if (category[i].second->IsPressed()) {

                        Game_Instance->selectCurrentProductColor(Colors[category[i].first]);
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
                        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,category[i].first);

                        Game_Instance->selectCurrentProductItem(category[i].first);
                        
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


