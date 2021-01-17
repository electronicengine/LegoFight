// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"



ULegoFightGameInstance::ULegoFightGameInstance()
{


}



void ULegoFightGameInstance::loadLoginLevel()
{
    UGameplayStatics::OpenLevel(this, "login_level", true);

}
