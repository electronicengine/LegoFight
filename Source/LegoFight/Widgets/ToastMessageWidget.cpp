// Fill out your copyright notice in the Description page of Project Settings.


#include "ToastMessageWidget.h"

bool UToastMessageWidget::Initialize()
{
	
	Super::Initialize();

	return true;
}

void UToastMessageWidget::setTextAndDisplay(const FString& Str, int TimeOut)
{
	Text_Message->SetText(FText::FromString(Str));
	Text_Message->SetVisibility(ESlateVisibility::Visible);
	Time_Issued = time(NULL) + TimeOut;
}

void UToastMessageWidget::hideText()
{
	Text_Message->SetVisibility(ESlateVisibility::Hidden);

}

void UToastMessageWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	if (Time_Issued == time(NULL))
		hideText();
}
