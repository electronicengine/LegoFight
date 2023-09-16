// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "../Bricks/Lego1x1Comp.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include <vector>
#include <memory>

#include "InventoryWidget.generated.h"


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

    std::vector<std::pair<int, UButton*>> GeneralBrick_Buttons;
    std::vector<std::pair<int, UButton*>> WeaponBrick_Buttons;
    std::vector<std::pair<int, UButton*>> VehicleBrick_Buttons;
    std::vector<std::pair<int, UButton*>> BuildingBrick_Buttons;
    std::vector<std::pair<int, UButton*>> Color_Buttons;


    std::vector<std::pair<int, UButton*>> *Current_Category_Buttons;



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

    UPROPERTY(meta =(BindWidget))
    UButton *Button_Cannon2x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_MachineGun2x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Hammer2x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Ax2x2;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_CarFender4x2;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_CarChassis1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_CarFar1x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_CarHook1x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_CompleteTrack;
    UPROPERTY(meta = (BindWidget))
        UButton* Button_CarHood2x2;
    UPROPERTY(meta = (BindWidget))
        UButton* Button_CarSeat2x2;
    UPROPERTY(meta = (BindWidget))
        UButton* Button_CarBodyWork2x1;
    UPROPERTY(meta = (BindWidget))
        UButton* Button_Embrasure2x1;
    
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Low1x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_High1x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Low2x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_High2x1;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Low4x1;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Low2x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_High2x2;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Low4x2;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_High4x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Low4x4;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_High4x4;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Low6x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_High6x2;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_Low6x4;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_High6x4;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_Blend2x2;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Trapezoid2x2;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Triangle2x2;

    UPROPERTY(meta = (BindWidget))
    UButton* Button_White;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Gray;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Black;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Yellow;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Green;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Red;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Purple;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Orange;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Brown;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Blue;
    UPROPERTY(meta = (BindWidget))
    UButton* Button_Cyan;

    //UFUNCTION()
    //void Button_2x1CarFenderClicked();
  
    BrickMenuCategory Current_Category;

    void removeWidget();
    void checkCategory();
    void setVisibilityCategoryButtons(const std::vector<std::pair<int, UButton*>> &CategoryButtons, bool Visible);

    public:
        virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


};
