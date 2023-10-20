// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterWidget.h"
#include "../Characters/LegoCharacter.h"
#include "../Vehicles/LegoCarChasis.h"


bool UCharacterWidget::Initialize()
{
	Super::Initialize();

	Button_GetBrick->OnClicked.AddDynamic(this, &UCharacterWidget::Button_GetBrickClicked);
	Button_Save->OnClicked.AddDynamic(this, &UCharacterWidget::Button_SaveClicked);
	Button_Plug->OnClicked.AddDynamic(this, &UCharacterWidget::Button_PlugClicked);
	Button_Interact->OnClicked.AddDynamic(this, &UCharacterWidget::Button_InteractClicked);
	Button_Fire->OnClicked.AddDynamic(this, &UCharacterWidget::Button_FireClicked);
	Button_Inventory->OnClicked.AddDynamic(this, &UCharacterWidget::Button_InventoryClicked);
	Button_Aim->OnClicked.AddDynamic(this, &UCharacterWidget::Button_AimClicked);
	Button_ZoomIn->OnClicked.AddDynamic(this, &UCharacterWidget::Button_ZoomInClicked);
	Button_ZoomOut->OnClicked.AddDynamic(this, &UCharacterWidget::Button_ZoomOutClicked);
	Button_LocationOffset->OnClicked.AddDynamic(this, &UCharacterWidget::Button_LocationOffsetClicked);
	Button_RotationOffset->OnClicked.AddDynamic(this, &UCharacterWidget::Button_RotationOffsetClicked);
	Button_Jump->OnClicked.AddDynamic(this, &UCharacterWidget::Button_JumpClicked);


	Button_GetBrick->SetVisibility(ESlateVisibility::Hidden);
	Button_Save->SetVisibility(ESlateVisibility::Hidden);
	Button_Plug->SetVisibility(ESlateVisibility::Hidden);
	Button_Interact->SetVisibility(ESlateVisibility::Hidden);
	Button_Fire->SetVisibility(ESlateVisibility::Hidden);
	Button_Inventory->SetVisibility(ESlateVisibility::Hidden);
	Button_Aim->SetVisibility(ESlateVisibility::Visible);
	Button_ZoomIn->SetVisibility(ESlateVisibility::Hidden);
	Button_ZoomOut->SetVisibility(ESlateVisibility::Hidden);
	Button_LocationOffset->SetVisibility(ESlateVisibility::Hidden);
	Button_RotationOffset->SetVisibility(ESlateVisibility::Hidden);
	Button_Jump->SetVisibility(ESlateVisibility::Visible);

	return true;
}

void UCharacterWidget::Button_GetBrickClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->buyBrick();
		Button_GetBrick->SetVisibility(ESlateVisibility::Visible);
		Button_Save->SetVisibility(ESlateVisibility::Hidden);
		Button_Plug->SetVisibility(ESlateVisibility::Visible);
		Button_Interact->SetVisibility(ESlateVisibility::Hidden);
		Button_Fire->SetVisibility(ESlateVisibility::Hidden);
		Button_Inventory->SetVisibility(ESlateVisibility::Visible);
		Button_Aim->SetVisibility(ESlateVisibility::Visible);
		Button_ZoomIn->SetVisibility(ESlateVisibility::Hidden);
		Button_ZoomOut->SetVisibility(ESlateVisibility::Hidden);
		Button_LocationOffset->SetVisibility(ESlateVisibility::Visible);
		Button_RotationOffset->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCharacterWidget::Button_SaveClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->saveAndLoad();
	}
}

void UCharacterWidget::Button_PlugClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->plugObject();

		Button_GetBrick->SetVisibility(ESlateVisibility::Visible);
		Button_Save->SetVisibility(ESlateVisibility::Hidden);
		Button_Plug->SetVisibility(ESlateVisibility::Hidden);
		Button_Interact->SetVisibility(ESlateVisibility::Hidden);
		Button_Fire->SetVisibility(ESlateVisibility::Visible);
		Button_Inventory->SetVisibility(ESlateVisibility::Visible);
		Button_Aim->SetVisibility(ESlateVisibility::Visible);
		Button_ZoomIn->SetVisibility(ESlateVisibility::Hidden);
		Button_ZoomOut->SetVisibility(ESlateVisibility::Hidden);
		Button_LocationOffset->SetVisibility(ESlateVisibility::Hidden);
		Button_RotationOffset->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UCharacterWidget::Button_InteractClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->equip();
	}
	else {

		ALegoCarChasis* vehicle = Cast<ALegoCarChasis>(GetOwningPlayerPawn());
		if (vehicle) {
			vehicle->exitCar();
		}
	}

}

void UCharacterWidget::Button_FireClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->fire();

	}
	else {
		ALegoCarChasis* vehicle = Cast<ALegoCarChasis>(GetOwningPlayerPawn());
		if (vehicle)
			vehicle->fire();
	}



}

void UCharacterWidget::Button_InventoryClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->openInventoryWidget();
		Button_GetBrick->SetVisibility(ESlateVisibility::Visible);
		Button_Save->SetVisibility(ESlateVisibility::Hidden);
		Button_Plug->SetVisibility(ESlateVisibility::Hidden);
		Button_Interact->SetVisibility(ESlateVisibility::Hidden);
		Button_Fire->SetVisibility(ESlateVisibility::Hidden);
		Button_Inventory->SetVisibility(ESlateVisibility::Visible);
		Button_Aim->SetVisibility(ESlateVisibility::Visible);
	}

}

void UCharacterWidget::Button_AimClicked()
{

	ALegoCharacter *character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->aimStart();
		if (character->Aiming_) {
			Button_GetBrick->SetVisibility(ESlateVisibility::Hidden);
			Button_Save->SetVisibility(ESlateVisibility::Visible);

			if (character->Keeping_Bricks) {
				Button_Plug->SetVisibility(ESlateVisibility::Visible);
				Button_LocationOffset->SetVisibility(ESlateVisibility::Visible);
				Button_RotationOffset->SetVisibility(ESlateVisibility::Visible);
			}
			else {
				Button_Plug->SetVisibility(ESlateVisibility::Hidden);
				Button_LocationOffset->SetVisibility(ESlateVisibility::Hidden);
				Button_RotationOffset->SetVisibility(ESlateVisibility::Hidden);
			}

			Button_Interact->SetVisibility(ESlateVisibility::Hidden);
			Button_Fire->SetVisibility(ESlateVisibility::Visible);
			Button_Inventory->SetVisibility(ESlateVisibility::Visible);
			Button_Aim->SetVisibility(ESlateVisibility::Visible);
			Button_ZoomIn->SetVisibility(ESlateVisibility::Visible);
			Button_ZoomOut->SetVisibility(ESlateVisibility::Visible);
			Button_LocationOffset->SetVisibility(ESlateVisibility::Hidden);
			Button_RotationOffset->SetVisibility(ESlateVisibility::Hidden);
			Button_Jump->SetVisibility(ESlateVisibility::Hidden);

		}
		else {
			Button_GetBrick->SetVisibility(ESlateVisibility::Hidden);
			Button_Save->SetVisibility(ESlateVisibility::Hidden);
			Button_Plug->SetVisibility(ESlateVisibility::Hidden);
			Button_Interact->SetVisibility(ESlateVisibility::Hidden);
			Button_Fire->SetVisibility(ESlateVisibility::Hidden);
			Button_Inventory->SetVisibility(ESlateVisibility::Hidden);
			Button_Aim->SetVisibility(ESlateVisibility::Visible);
			Button_ZoomIn->SetVisibility(ESlateVisibility::Hidden);
			Button_ZoomOut->SetVisibility(ESlateVisibility::Hidden);
			Button_LocationOffset->SetVisibility(ESlateVisibility::Hidden);
			Button_RotationOffset->SetVisibility(ESlateVisibility::Hidden);
			Button_Jump->SetVisibility(ESlateVisibility::Visible);

		}
	}

}

void UCharacterWidget::Button_ZoomInClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->zoom(5);
	}
}

void UCharacterWidget::Button_ZoomOutClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->zoom(-5);
	}
}

void UCharacterWidget::Button_LocationOffsetClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->giveOffsetLocation();
	}
}

void UCharacterWidget::Button_RotationOffsetClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->giveOffsetRotation();
	}
}

void UCharacterWidget::Button_JumpClicked()
{
	ALegoCharacter* character = Cast<ALegoCharacter>(GetOwningPlayerPawn());
	if (character) {
		character->Jump();
	}

	ALegoCarChasis* vehicle = Cast<ALegoCarChasis>(GetOwningPlayerPawn());
	if (vehicle)
		vehicle->jump();
	
}

void UCharacterWidget::setInteractButtonVisibilty(ESlateVisibility Val)
{
	Button_Interact->SetVisibility(Val);
}

void UCharacterWidget::setFireButtonVisibilty(ESlateVisibility Val)
{
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString("setFireButtonVisibilty"));

	Button_Fire->SetVisibility(Val);
}

void UCharacterWidget::setAimButtonVisibilty(ESlateVisibility Val)
{
	Button_Aim->SetVisibility(Val);
}
