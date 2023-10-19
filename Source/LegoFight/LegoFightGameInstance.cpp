// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/Button.h"
#include "Vehicles/EnemyVehicleAIController.h"
#include "Widgets/SaveAndLoadGameWidget.h"
#include "LegoFightSaveGame.h"

ULegoFightGameInstance::ULegoFightGameInstance()
{

    Selected_Item = "High2x2";
    Invantory_Panel = nullptr;
    Char_Panel = nullptr;

    TArray<UObject*> general_brick_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/bricks/meshes"), general_brick_assets, EngineUtils::ATL_Regular);
    for (auto asset : general_brick_assets)
    {
        General_Brick_Meshes[asset->GetName()] = Cast<UStaticMesh>(asset);
    }

    TArray<UObject*> weapon_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/weapons/meshes"), weapon_assets, EngineUtils::ATL_Regular);
    for (auto asset : weapon_assets)
    {
        Weapon_Meshes[asset->GetName()] = Cast<UStaticMesh>(asset);
    }

    TArray<UObject*> vehicle_assets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/bricks/meshes/vehicle"), vehicle_assets, EngineUtils::ATL_Regular);
    for (auto asset : vehicle_assets)
    {
        General_Brick_Meshes[asset->GetName()] = Cast<UStaticMesh>(asset);
    }

    // default material
    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    Default_Brick_Material = material_asset.Object;

    initializeItemOptions();
    initializeDestructables();


}

void ULegoFightGameInstance::initializeItemOptions()
{
        
    for (auto mesh : General_Brick_Meshes)
    {
        Item_Options[mesh.first] = { mesh.first, Selected_Color, General_Brick_Meshes[mesh.first], Default_Brick_Material,10 };
    }

    for (auto mesh : Weapon_Meshes)
    {
        Item_Options[mesh.first] = { mesh.first, Selected_Color, Weapon_Meshes[mesh.first], Default_Brick_Material, 100 };
    }

   
}

void ULegoFightGameInstance::initializeDestructables()
{

    for (std::pair<FString, UStaticMesh*> var : General_Brick_Meshes) {
        TArray<UObject*> MeshAssets;
        FString meshes_path = "/Game/bricks/destructables/";
        meshes_path += var.first;

        EngineUtils::FindOrLoadAssetsByPath(meshes_path, MeshAssets, EngineUtils::ATL_Regular);
        int i = 0;
        for (auto asset : MeshAssets)
        {
            UStaticMesh* mesh = Cast<UStaticMesh>(asset);
            Destructables[var.first].Push(mesh);
        }
    }

    FString meshes_path = "/Game/bricks/destructables/";
    meshes_path += "lego_car1";
    TArray<UObject*> MeshAssets;

    EngineUtils::FindOrLoadAssetsByPath(meshes_path, MeshAssets, EngineUtils::ATL_Regular);
    int i = 0;
    for (auto asset : MeshAssets)
    {
        UStaticMesh* mesh = Cast<UStaticMesh>(asset);
        Destructables["lego_car1"].Push(mesh);
    }
}



UUserWidget* ULegoFightGameInstance::savePanel(ALegoCarChasis* Vehicle, FVector ImpactPoint)
{
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ULegoFightSaveGame* save_game = Cast<ULegoFightSaveGame>(UGameplayStatics::CreateSaveGameObject(ULegoFightSaveGame::StaticClass()));

    Save_Vehicle = Vehicle;
    Impact_Point = ImpactPoint;

    if(Save_Vehicle)
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Save_Vehicle ok"));


    if (Save_Panel == nullptr)
    {
        //if (Char_Panel != nullptr)
        //{
        //    Char_Panel->RemoveFromParent();
        //    Char_Panel = nullptr;
        //}

        Save_Panel = CreateWidget<UUserWidget>(this, Save_Panel_Container);

        Save_Panel->AddToViewport();


        controller->bShowMouseCursor = true;
        controller->bEnableClickEvents = true;
        controller->bEnableMouseOverEvents = true;
        controller->bEnableTouchEvents = true;

        Save_Panel->SetVisibility(ESlateVisibility::Visible);

        controller->SetInputMode(FInputModeUIOnly());
    }
    else {

        if (Save_Panel->GetVisibility() == ESlateVisibility::Visible)
        {
            Save_Panel->SetVisibility(ESlateVisibility::Collapsed);
            controller->SetInputMode(FInputModeGameOnly());
            controller->bShowMouseCursor = false;
        }
        else {
            Save_Panel->SetVisibility(ESlateVisibility::Visible);
            controller->SetInputMode(FInputModeUIOnly());

            controller->bShowMouseCursor = true;
        }

    }

    return Save_Panel;
}

UUserWidget *ULegoFightGameInstance::loadInvantoryPanel()
{
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if(Invantory_Panel == nullptr)
    {
   /*     if(Char_Panel != nullptr)
        {
            Char_Panel->RemoveFromParent();
            Char_Panel = nullptr;
        }*/

        Invantory_Panel = CreateWidget<UUserWidget>(this, Invantory_Panel_Container);

        Invantory_Panel->AddToViewport();


        controller->bShowMouseCursor = true;
        controller->bEnableClickEvents = true;
        controller->bEnableMouseOverEvents = true;
        controller->bEnableTouchEvents = true;

        Invantory_Panel->SetVisibility(ESlateVisibility::Visible);

        controller->SetInputMode(FInputModeUIOnly());
    }
    else {

        if (Invantory_Panel->GetVisibility() == ESlateVisibility::Visible)
        {
            Invantory_Panel->SetVisibility(ESlateVisibility::Collapsed);
            controller->SetInputMode(FInputModeGameOnly());
            controller->bShowMouseCursor = false;
        }
        else {
            Invantory_Panel->SetVisibility(ESlateVisibility::Visible);
            controller->SetInputMode(FInputModeUIOnly());

            controller->bShowMouseCursor = true;
        }

    }



    return Invantory_Panel;
}



UUserWidget *ULegoFightGameInstance::loadCharacterPanel()
{

    if(Char_Panel == nullptr)
    {
        if(Invantory_Panel != nullptr)
        {
            Invantory_Panel->RemoveFromParent();
            Invantory_Panel = nullptr;
        }

        Char_Panel = CreateWidget<UUserWidget>(this, Character_Panel_Container);

        if(Char_Panel != nullptr)
            Char_Panel->AddToViewport();

        APlayerController *controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

        controller->bShowMouseCursor = false;
    //    controller->bEnableClickEvents = false;
    //    controller->bEnableMouseOverEvents = false;

    }

    return Char_Panel;
}



ABrick *ULegoFightGameInstance::spawnBrick(const FString& Name, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{

    ABrick *spawn_brick;

    spawn_brick = GetWorld()->SpawnActor<ABrick>(ABrick::StaticClass(), SpawnLocation, SpawnRotation);
    Item_Options[Name].Color = Selected_Color;
    spawn_brick->setBrickTypeOptions(Item_Options[Name]);

   

    return spawn_brick;

}


ALegoCarChasis *ULegoFightGameInstance::spawnVehicle(const FString& Name, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{
    ALegoCarChasis *vehicle;

    if (Name.Find(ENEMY_APPENDIX) >= 0) {

        vehicle = GetWorld()->SpawnActor<AEnemyLegoVehicle>(AEnemyLegoVehicle::StaticClass(), SpawnLocation + FVector(0, 0, 100.0f), SpawnRotation);


    }
    else {
        vehicle = GetWorld()->SpawnActor<ALegoCarChasis>(ALegoCarChasis::StaticClass(), SpawnLocation + FVector(0, 0, 100.0f), SpawnRotation);

    }

    return vehicle;
    
}

AWeapon* ULegoFightGameInstance::spawnWeapon(const FString& Name, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    AWeapon* weapon;
    weapon = GetWorld()->SpawnActor<AWeapon>(AWeapon::StaticClass(), SpawnLocation + FVector(0, 0, 100.0f), SpawnRotation);
    Item_Options[Name].Color = Selected_Color;
    weapon->setBrickTypeOptions(Item_Options[Name]);
    return weapon;
}

AActor* ULegoFightGameInstance::spawnItem(const FVector& SpawnLocation, const FRotator& SpawnRotation, const FString& Name)
{
    FString item_name;
    AWeapon* weapon;
    ALegoCarChasis* vehicle;
    ABrick* brick;

    if (Name.GetAllocatedSize() != 0)
        item_name = Name;
    else
        item_name = Selected_Item;

    //weapon
    if (item_name.Find(WEAPON_APPENDIX) > 0) {

        weapon = spawnWeapon(item_name, SpawnLocation, SpawnRotation);
        return weapon;
    }
    else if (item_name.Find(VEHICLE_APPENDIX) > 0) {
        vehicle = spawnVehicle(item_name, SpawnLocation, SpawnRotation);
        return vehicle;
    }
    else {
        brick = spawnBrick(item_name, SpawnLocation, SpawnRotation);
        return brick;
    }

    return nullptr;
}



void ULegoFightGameInstance::selectCurrentProductItem(const FString & ItemName)
{
    Selected_Item = ItemName;
    
}

void ULegoFightGameInstance::selectCurrentProductColor(FLinearColor Color)
{
    Selected_Color = Color;
}

bool ULegoFightGameInstance::saveGame(FString Name)
{
    ULegoFightSaveGame* save_game = Cast<ULegoFightSaveGame>(UGameplayStatics::CreateSaveGameObject(ULegoFightSaveGame::StaticClass()));

    if(Save_Vehicle)
    {
        ConstructionInfo info = Save_Vehicle->compileConstructInfo(Save_Vehicle);

        TSharedPtr<FJsonObject> json = save_game->convertConstructionInfoToJson(info);

        save_game->SaveObject = save_game->serializeJsonObject(json);

        UGameplayStatics::SaveGameToSlot(save_game, Name, 0);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("saved"));
        return true;

    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Save_Vehicle non"));

        return false;
    }

}

bool ULegoFightGameInstance::loadGame(FString Name)
{

    //    construction_info = save_game->Construction_Info;
    ULegoFightGameInstance* game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

    ULegoFightSaveGame* save_game = Cast<ULegoFightSaveGame>(UGameplayStatics::CreateSaveGameObject(ULegoFightSaveGame::StaticClass()));
    save_game = Cast<ULegoFightSaveGame>(UGameplayStatics::LoadGameFromSlot(Name, 0));
    if (save_game) {
        TSharedPtr<FJsonObject> de_json = save_game->deserializeJsonObject(save_game->SaveObject);
        ConstructionInfo de_info = save_game->convertJsonToConstructionInfo(de_json);

        IBuiltInInterface::buildFromConstructionInfo(de_info, Impact_Point, game_instance);

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("loaded"));
    }
    else {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, Name);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "save game non");

    }


    return false;
}
