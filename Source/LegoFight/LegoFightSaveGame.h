// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Vehicles/EnemyLegoVehicle.h"
#include "Interfaces/BuiltInInterface.h"
#include "Json.h"

#include "LegoFightSaveGame.generated.h"


/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ULegoFightSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    ULegoFightSaveGame();

//    UPROPERTY(EditAnywhere)
    ConstructionInfo Construction_Info;

    UPROPERTY(EditAnywhere)
    FString SaveObject;


    TSharedPtr<FJsonObject> convertConstructionInfoToJson(const ConstructionInfo &Info);
    FString serializeJsonObject(const TSharedPtr<FJsonObject> &Json);

    ConstructionInfo convertJsonToConstructionInfo(const TSharedPtr<FJsonObject> &Json);
    TSharedPtr<FJsonObject> deserializeJsonObject(const FString &JsonString);
};
