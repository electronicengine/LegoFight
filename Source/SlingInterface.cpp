// Fill out your copyright notice in the Description page of Project Settings.


#include "SlingInterface.h"

// Include necessary UE4 headers
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
// Add default functionality here for any ISlingInterface functions that are not pure virtual.

ISlingInterface::ISlingInterface()
{
	Sling_Mode_Enable = false;

}
