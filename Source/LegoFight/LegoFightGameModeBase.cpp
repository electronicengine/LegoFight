// Copyright Epic Games, Inc. All Rights Reserved.


#include "LegoFightGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ALegoFightGameModeBase::ALegoFightGameModeBase()
{

    UE_LOG(LogTemp, Warning, TEXT("ALegoFightGameModeBase"));


}



void ALegoFightGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ALegoFightGameModeBase::BeginPlay"));


//    UGameplayStatics::OpenLevel(this, "login_level", true);
//    Controller_ = UGameplayStatics::GetPlayerController(GetWorld(), 0);

//    FInputModeGameOnly input_mode;
//    Controller_->SetInputMode(input_mode);
}
