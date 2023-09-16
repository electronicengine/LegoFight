// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Brick.h"
#include "Bricks/Lego1x1Trapezoid.h"
#include "Bricks/Lego1x1Triangle.h"
#include "Bricks/Lego1x1Comp.h"
#include "Bricks/Lego2x1Comp.h"
#include "Bricks/Lego2x2Comp.h"
#include "Bricks/Lego3x1Comp.h"
#include "Bricks/Lego3x2Comp.h"
#include "Bricks/Lego1x1Semi.h"
#include "Bricks/Lego2x1Semi.h"
#include "Bricks/Lego2x2Semi.h"
#include "Bricks/Lego3x1Semi.h"
#include "Bricks/Lego3x2Semi.h"
#include "Guns/FireWeapon.h"
#include "Guns/MeleeWeapon.h"
#include "Guns/PhysicsWeapon.h"
#include "Vehicles/EnemyLegoVehicle.h"
#include <map>

#include "LegoFightGameInstance.generated.h"


struct BrickOptions {

    FString Name;
    FLinearColor Color;
    UStaticMesh* Mesh;
    UMaterial* Material;
    FVector CollisionBox_Location;
    FVector CollisionBox_Scale3D;
    FVector Plug_Pivot_Location;
    int Pivot_Height;
    int Pivot_Width;
    int Health;

    int Height_Offset;
    FVector SidePlug_Pivot_Location;

    BrickType Type;
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

    BrickType Selected_Brick;
    FLinearColor Selected_Color;

    UPROPERTY(EditAnywhere, Category = "Vehicle")
    TSubclassOf<AEnemyLegoVehicle> Lego_Enemy_Vehicle_Container;
    UPROPERTY(EditAnywhere, Category = "Vehicle")
    TSubclassOf<AEnemyLegoVehicle> Lego_Vehicle_Container;

    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego1x1Trapezoid> Lego1x1Trapezoid_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego1x1Triangle> Lego1x1Triangle_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego1x1Comp> Lego1x1Comp_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego2x1Comp> Lego2x1Comp_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego2x2Comp> Lego2x2Comp_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego3x1Comp> Lego3x1Comp_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego3x2Comp> Lego3x2Comp_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego1x1Semi> Lego1x1Semi_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego2x1Semi> Lego2x1Semi_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego2x2Semi> Lego2x2Semi_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego3x1Semi> Lego3x1Semi_Container;
    UPROPERTY(EditAnywhere, Category = "General")
    TSubclassOf<ALego3x2Semi> Lego3x2Semi_Container;

    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AFireWeapon> Cannon_Container;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AFireWeapon> Machine_Gun_Container;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AMeleeWeapon> Hammer_Container;
    UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<AMeleeWeapon> Ax_Container;

    std::map<BrickType, FString> BrickType_Names;
    std::map<FString, UStaticMesh*> Brick_Meshes;
    std::map<BrickType, BrickOptions> Brick_Options;

    UMaterial *Default_Brick_Material;
    

public:
    ULegoFightGameInstance();

    void initializeBrickOptions();



    UUserWidget *loadInvantoryPanel();
    UUserWidget *loadCharacterPanel();
    ABrick *spawnBrick(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation);
    ALegoCarChasis *spawnVehicle(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation);

    void selectCurrentProductBrick(BrickType Type);
    void selectCurrentProductBrickColor(FLinearColor Color);

    UTexture2D* texture;

};
