// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include <vector>
#include "BuiltInInterface.generated.h"

class ABrick;
class ULegoFightGameInstance;

struct ConstructionInfo
{
    int Plugin_Index;
    FRotator Offset_Rotation;
    FVector Offset_Location;
    FString Item_Name;
    FLinearColor Item_Color;
    class AActor *Item;
    std::vector<struct ConstructionInfo> Plugged_Items;
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuiltInInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LEGOFIGHT_API IBuiltInInterface
{
	GENERATED_BODY()




	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
    ConstructionInfo compileConstructInfo(AActor *Object);
    static void buildFromConstructionInfo(const ConstructionInfo &Info, const FVector &BuildLocation,
                                          ULegoFightGameInstance *GameInstance, ABrick *MainBrick = nullptr);
	
};
