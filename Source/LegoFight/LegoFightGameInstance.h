// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LegoFightGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ULegoFightGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    ULegoFightGameInstance();

    void loadLoginLevel();
	
};
