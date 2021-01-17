// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LegoFightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ALegoFightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

    public:

    ALegoFightGameModeBase();

    void BeginPlay();

};
