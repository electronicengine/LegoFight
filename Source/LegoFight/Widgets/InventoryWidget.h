// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "../Bricks/Lego1x1Comp.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

    virtual bool Initialize();

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel *Panel_;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_CannonComp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_MachineGunComp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_HammerComp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_AxComp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_1x1Trapezoid;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_1x1Triangle;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_1x1Comp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_2x1Comp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_2x2Comp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_3x1Comp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_3x2Comp;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_1x1Semi;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_2x1Semi;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_2x2Semi;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_3x1Semi;
    UPROPERTY(meta =(BindWidget))
    UButton *Button_3x2Semi;

    UFUNCTION()
    void Button_1x1TrapezoidClicked();
    UFUNCTION()
    void Button_1x1TriangleClicked();
    UFUNCTION()
    void Button_1x1CompClicked();
    UFUNCTION()
    void Button_2x1CompClicked();
    UFUNCTION()
    void Button_2x2CompClicked();
    UFUNCTION()
    void Button_3x1CompClicked();
    UFUNCTION()
    void Button_3x2CompClicked();
    UFUNCTION()
    void Button_1x1SemiClicked();
    UFUNCTION()
    void Button_2x1SemiClicked();
    UFUNCTION()
    void Button_2x2SemiClicked();
    UFUNCTION()
    void Button_3x1SemiClicked();
    UFUNCTION()
    void Button_3x2SemiClicked();
    UFUNCTION()
    void Button_CannonCompClicked();
    UFUNCTION()
    void Button_MachineGunCompClicked();
    UFUNCTION()
    void Button_HammerCompClicked();
    UFUNCTION()
    void Button_AxCompClicked();

    void removeWidget();



};
