// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Brick.h"
#include "Guns/Weapon.h"
#include "Vehicles/EnemyLegoVehicle.h"
#include <map>

#include "LegoFightGameInstance.generated.h"


#define WEAPON_APPENDIX         "weapon"
#define FIRE_WEAPON_APPENDIX    "fire"
#define MELEE_WEAPON_APPENDIX   "melee"
#define VEHICLE_APPENDIX        "vehicle"
#define ENEMY_APPENDIX          "enemy"
#define SIDED_APPENDIX          "sided"
#define LOW_APPENDIX            "Low"
#define HIGH_APPENDIX           "High"





struct ItemOptions {

    FString Name;
    FLinearColor Color;
    UStaticMesh* Mesh;
    UMaterial* Material;
    int Health;

    ItemType Type;
    BrickSubType SubType;
};

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API ULegoFightGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> Invantory_Panel_Container;

    UUserWidget* Invantory_Panel;
    UUserWidget* Char_Panel;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<class UUserWidget> Character_Panel_Container;

    FString Selected_Item;
    FLinearColor Selected_Color;



    std::map<FString, UStaticMesh*> General_Brick_Meshes;
    std::map<FString, UStaticMesh*> Weapon_Meshes;
    std::map<FString, UStaticMesh*> Vehicle_Meshes;

    std::map<FString, ItemOptions> Item_Options;

    UMaterial *Default_Brick_Material;
    

public:
    ULegoFightGameInstance();

    void initializeItemOptions();



    UUserWidget *loadInvantoryPanel();
    UUserWidget *loadCharacterPanel();
    ABrick* spawnBrick(const FString& Name, const FVector &SpawnLocation, const FRotator &SpawnRotation);
    ALegoCarChasis *spawnVehicle(const FString& Name, const FVector &SpawnLocation, const FRotator &SpawnRotation);
    AWeapon* spawnWeapon(const FString& Name, const FVector& SpawnLocation, const FRotator& SpawnRotation);
    AActor* spawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation, const FString& Name = "");

    void selectCurrentProductItem(const FString& ItemName);
    void selectCurrentProductColor(FLinearColor Color);

    UTexture2D* texture;

};
