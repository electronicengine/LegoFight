// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include <vector>
#include <memory>
#include <map>

#include "InventoryWidget.generated.h"


class ULegoFightGameInstance;
enum BrickColor;

enum BrickMenuCategory {
    General,
    Weapon,
    Vehicle,
    Building
};
/**
 * 
 */
UCLASS()
class LEGOFIGHT_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()

    virtual bool Initialize();

    ULegoFightGameInstance* Game_Instance;

    std::vector<std::pair<FString, UButton*>> GeneralBrick_Buttons;
    std::vector<std::pair<FString, UButton*>> WeaponBrick_Buttons;
    std::vector<std::pair<FString, UButton*>> VehicleBrick_Buttons;
    std::vector<std::pair<FString, UButton*>> BuildingBrick_Buttons;
    std::vector<std::pair<FString, UButton*>> Color_Buttons;


    std::vector<std::pair<FString, UButton*>> *Current_Category_Buttons;



    std::vector<std::pair<BrickMenuCategory, UButton*>> Category_Buttons;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel *Panel_;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_General_Category;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Weapon_Category;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Vehicle_Category;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Building_Category;

    UPROPERTY(meta = (BindWidget))
        UScrollBox* box;
    

    //UFUNCTION()
    //void Button_2x1CarFenderClicked();
  
    BrickMenuCategory Current_Category;

    std::map<FString, FLinearColor> Colors;

    void removeWidget();
    void checkCategory();
    void setVisibilityCategoryButtons(const std::vector<std::pair<FString, UButton*>> &CategoryButtons, bool Visible);

    public:
        virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


};
