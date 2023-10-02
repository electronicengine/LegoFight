// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveAndLoadGameWidget.h"
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


bool USaveAndLoadGameWidget::Initialize()
{

	Super::Initialize();
	// Button_1x1Triangle->OnClicked.AddDynamic(this, &UInventoryWidget::Button_1x1TriangleClicked);
	Button_Save->OnClicked.AddDynamic(this, &USaveAndLoadGameWidget::onClickedButton_Save);
	Button_Load->OnClicked.AddDynamic(this, &USaveAndLoadGameWidget::onClickedButton_Load);

	return true;
}

void USaveAndLoadGameWidget::removeWidget()
{

	SetVisibility(ESlateVisibility::Collapsed);
	ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

	game_instance->loadCharacterPanel();
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	controller->SetInputMode(FInputModeGameOnly());
	controller->bShowMouseCursor = false;

}

void USaveAndLoadGameWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
}

void USaveAndLoadGameWidget::onClickedButton_Save()
{

	FText txt = save_name->GetText();
	ULegoFightGameInstance* game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

	game_instance->saveGame(txt.ToString());

	removeWidget();

}

void USaveAndLoadGameWidget::onClickedButton_Load()
{
	FText txt = save_name->GetText();
	ULegoFightGameInstance* game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

	game_instance->loadGame(txt.ToString());

	removeWidget();
}
