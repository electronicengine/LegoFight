// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "ToastMessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API UToastMessageWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual bool Initialize();


	int Time_Issued;
public:

	void setTextAndDisplay(const FString& Str, int TimeOut);
	void hideText();

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text_Message;


};
