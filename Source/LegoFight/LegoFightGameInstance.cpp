// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ULegoFightGameInstance::ULegoFightGameInstance()
{

    Selected_Brick = High2x2;
    Invantory_Panel = nullptr;
    Char_Panel = nullptr;
    static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("Texture2D'/Game/widgets/thumbnails/ax.ax'"));
    texture = Texture.Object;


    TArray<UObject*> MeshAssets;
    EngineUtils::FindOrLoadAssetsByPath(FString("/Game/bricks/meshes"), MeshAssets, EngineUtils::ATL_Regular);

    //upload_brick_meshes
    for (auto asset : MeshAssets)
    {
        Brick_Meshes[asset->GetName()] = Cast<UStaticMesh>(asset);
    }

    // default material
    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    Default_Brick_Material = material_asset.Object;

    initializeBrickOptions();
}

void ULegoFightGameInstance::initializeBrickOptions()
{



    Brick_Options[Low1x1] = { "Low1x1", Selected_Color, Brick_Meshes["Low1x1"], Default_Brick_Material,
        FVector(-12.439331f, 12.596649f, 4.895172f),
        FVector(0.373471f, 0.373044f, 0.152122f),
        FVector(25, -25.0f, 30.7f),3,3,10,20};

    Brick_Options[High1x1] = { "High1x1", Selected_Color, Brick_Meshes["High1x1"], Default_Brick_Material,
        FVector(-12.51495f, -12.462021f, -4.505463f),
        FVector(0.348109f, 0.359798f, 0.448889f),
        FVector(25, -25.0f, 30.7f),3,3,10,0};

    Brick_Options[Low2x1] = { "Low2x1", Selected_Color, Brick_Meshes["Low2x1"], Default_Brick_Material,
        FVector(-12.581482f, 0.117626f, 4.973537f),
        FVector(0.379243f, 0.773043f, 0.129856f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10,20};

    Brick_Options[High2x1] = { "High2x1", Selected_Color, Brick_Meshes["Low2x1"], Default_Brick_Material,
        FVector(-12.338715f, -0.316315f, -4.261749),
        FVector(0.370634f, 0.739971f, 0.428869f),
        FVector(25, -25.0f, 30.7f), 3, 3,10,0};

    Brick_Options[Low4x1] = { "Low4x1", Selected_Color, Brick_Meshes["Low4x1"], Default_Brick_Material,
        FVector(-12.575897f, -0.316315f, 4.995179f),
        FVector(0.370634f, 1.543776f, 0.14167f),
        FVector(75, -25, 31.5f), 3, 5, 10, 20};

    Brick_Options[Low2x2] = { "Low2x2", Selected_Color, Brick_Meshes["Low2x2"], Default_Brick_Material,
        FVector(0,0, 5.085279f),
        FVector(0.77f, 0.77f, 0.140004f),
        FVector(25, -25.0f, 31.5f), 3, 3, 10,20 };

    Brick_Options[High2x2] = { "High2x2", Selected_Color, Brick_Meshes["High2x2"], Default_Brick_Material,
        FVector(0, 0, -5),
        FVector(0.77f, 0.77f, 0.448889f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10,0};

    Brick_Options[Low4x2] = { "Low4x2", Selected_Color, Brick_Meshes["Low4x2"], Default_Brick_Material,
        FVector(-0.069672f,0, 5.026886f),
        FVector(0.757938f, 1.52058f, 0.14002f),
        FVector(75, -25, 31.5f), 3, 5, 10, 20};

    Brick_Options[High4x2] = { "High4x2", Selected_Color, Brick_Meshes["High4x2"], Default_Brick_Material,
        FVector(0, 0, -4),
        FVector(0.7f, -1.5f, 0.4f),
        FVector(25, -50.1f, 30.7f), 5, 3, 10, 0 };

    Brick_Options[Low4x4] = { "Low4x4", Selected_Color, Brick_Meshes["Low4x4"], Default_Brick_Material,
        FVector(25,-25, 2),
        FVector(1.55f, 1.55f, 0.12f),
        FVector(75, -75, 31.5f), 5, 5, 10, 20 };

    Brick_Options[High4x4] = { "High4x4", Selected_Color, Brick_Meshes["High4x4"], Default_Brick_Material,
        FVector(25,-25, 4),
        FVector(1.55f, 1.55f, 0.64f),
        FVector(75, -75, 50.7f), 5, 5, 10, 0 };

    Brick_Options[Low6x2] = { "Low6x2", Selected_Color, Brick_Meshes["Low6x2"], Default_Brick_Material,
        FVector(50,0, 2),
        FVector(2.31f, 0.77f, 0.12f),
        FVector(125, -25, 31.5), 3, 7, 10, 20};

    Brick_Options[High6x2] = { "High6x2", Selected_Color, Brick_Meshes["High6x2"], Default_Brick_Material,
        FVector(0,-50, 4),
        FVector(0.77f, 2.31f, 0.64f),
        FVector(25, -125, 50.7), 7, 3, 10, 0 };

    Brick_Options[Low6x4] = { "Low6x4", Selected_Color, Brick_Meshes["Low6x4"], Default_Brick_Material,
        FVector(50, -25, 2),
        FVector(2.31f, 1.55f, 0.12f),
        FVector(125, -75, 31.5f), 5, 7, 10, 20 };

    Brick_Options[High6x4] = { "High6x4", Selected_Color, Brick_Meshes["High6x4"], Default_Brick_Material,
        FVector(25, -50, 4),
        FVector(1.55f, 2.31f, 0.64f),
        FVector(75, -125, 50.7f), 7, 5, 10, 0 };

    Brick_Options[Trapezoid2x2] = { "Trapezoid2x2", Selected_Color, Brick_Meshes["Trapezoid2x2"], Default_Brick_Material,
        FVector(0,0,4),
        FVector(0.77f, 0.77f, 0.64f),
        FVector(25, 25, 50.7f), 1, 3, 10, 0 };

    Brick_Options[Triangle2x2] = { "Triangle2x2", Selected_Color, Brick_Meshes["Triangle2x2"], Default_Brick_Material,
        FVector(0,0,4),
        FVector(0.77f, 0.77f, 0.64f),
        FVector(0, 0, 0), 0, 0, 10, 0 };

    Brick_Options[Blend2x2] = { "Blend2x2", Selected_Color, Brick_Meshes["Blend2x2"], Default_Brick_Material,
        FVector(0, 0, -5),
        FVector(0.77f, 0.77f, 0.448889f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 0 };

    Brick_Options[CarHood2x2] = { "CarHood2x2", Selected_Color, Brick_Meshes["CarHood2x2"], Default_Brick_Material,
        FVector(0, 0, -5),
        FVector(0.77f, 0.77f, 0.448889f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 0 };

    Brick_Options[CarHood2x2] = { "CarHood2x2", Selected_Color, Brick_Meshes["CarHood2x2"], Default_Brick_Material,
        FVector(0, 0, -5),
        FVector(0.77f, 0.77f, 0.448889f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 10 };

    Brick_Options[CarHook1x1] = { "CarHook1x1", Selected_Color, Brick_Meshes["CarHook1x1"], Default_Brick_Material,
        FVector(-12.505371f, 23.310486f, 5.108871f),
        FVector(0.411496f, 0.694605f, 0.138981f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 20 };

    Brick_Options[CarFar1x1] = { "CarFar1x1", Selected_Color, Brick_Meshes["CarFar1x1"], Default_Brick_Material,
        FVector(12.581627f, -12.574644f, 5.828213f),
        FVector(0.304359f, 0.304641f, 0.133212f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 20 };

    Brick_Options[CarFender4x2] = { "CarFender4x2", Selected_Color, Brick_Meshes["CarFender4x2"], Default_Brick_Material,
        FVector(-0.001842f, 0, 5.585449f),
        FVector(0.627105f, 1.250133f, 0.12f),
        FVector(75, -25, 31.5f), 3, 5, 10, 20 };

    Brick_Options[CarBodyWork2x1] = { "CarBodyWork2x1", Selected_Color, Brick_Meshes["CarBodyWork2x1"], Default_Brick_Material,
        FVector(0, 15.113293f, -1.665622f),
        FVector(1.336335f, -0.456999f, 0.370121f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 20 };
    //setupSidePlugPoints(FVector(50.1f, 45, -13.85f), 3, 9);

    Brick_Options[CarBodyWork2x1] = { "CarBodyWork2x1", Selected_Color, Brick_Meshes["CarBodyWork2x1"], Default_Brick_Material,
        FVector(0, 15.113293f, -1.665622f),
        FVector(1.336335f, -0.456999f, 0.370121f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 20 };

    Brick_Options[Embrasure2x1] = { "Embrasure2x1", Selected_Color, Brick_Meshes["Embrasure2x1"], Default_Brick_Material,
        FVector(-12.494331f, 0, 5.968761f),
        FVector(0.373589f, 0.77f, 0.098602f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 15 };

    Brick_Options[Embrasure2x1] = { "Embrasure2x1", Selected_Color, Brick_Meshes["Embrasure2x1"], Default_Brick_Material,
        FVector(-12.494331f, 0, 5.968761f),
        FVector(0.373589f, 0.77f, 0.098602f),
        FVector(25, -25.0f, 30.7f), 3, 3, 10, 15 };

    Brick_Options[CarChassis1] = { "CarChassis1", Selected_Color, Brick_Meshes["CarChassis1"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(100.5f, -25.0f, 27.7f), 3, 9, 10, 0 };

    Brick_Options[CompleteTrack] = { "CompleteTrack", Selected_Color, Brick_Meshes["CompleteTrack"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 0 };

    Brick_Options[Hammer2x2] = { "Hammer2x2", Selected_Color, Brick_Meshes["Hammer2x2"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 20 };

    Brick_Options[Ax2x2] = { "Ax2x2", Selected_Color, Brick_Meshes["Ax2x2"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 20 };

    Brick_Options[MachineGun2x2] = { "MachineGun2x2", Selected_Color, Brick_Meshes["MachineGun2x2"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 20 };

    Brick_Options[Cannon2x2] = { "Cannon2x2", Selected_Color, Brick_Meshes["Cannon2x2"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 20 };

    Brick_Options[CarSeat2x2] = { "CarSeat2x2", Selected_Color, Brick_Meshes["CarSeat2x2"], Default_Brick_Material,
        FVector(0, 0, 0),
        FVector(0, 0, 0),
        FVector(0, 0,0), 0, 0, 1000, 20 };


}



UUserWidget *ULegoFightGameInstance::loadInvantoryPanel()
{
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if(Invantory_Panel == nullptr)
    {
        if(Char_Panel != nullptr)
        {
            Char_Panel->RemoveFromParent();
            Char_Panel = nullptr;
        }

        Invantory_Panel = CreateWidget<UUserWidget>(this, Invantory_Panel_Container);

        Invantory_Panel->AddToViewport();


        controller->bShowMouseCursor = true;
        controller->bEnableClickEvents = true;
        controller->bEnableMouseOverEvents = true;
        controller->bEnableTouchEvents = true;

        Invantory_Panel->SetVisibility(ESlateVisibility::Visible);

        //controller->SetInputMode(FInputModeUIOnly());
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
            controller->bShowMouseCursor = true;
        }

    }



    return Invantory_Panel;
}



UUserWidget *ULegoFightGameInstance::loadCharacterPanel()
{

    if(Char_Panel == nullptr)
    {
        GEngine->AddOnScreenDebugMessage(-1, -5 ,FColor::Orange, TEXT("loadCharacterPanel"));

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



ABrick *ULegoFightGameInstance::spawnBrick(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{

    ABrick *spawn_brick;
    int type;

    if(Type < 0)
        type = Selected_Brick;
    else
        type = Type;

    spawn_brick = GetWorld()->SpawnActor<ABrick>(ABrick::StaticClass(), SpawnLocation, SpawnRotation);
    spawn_brick->setBrickTypeOptions(Brick_Options[(BrickType)type]);

    return spawn_brick;

}


ALegoCarChasis *ULegoFightGameInstance::spawnVehicle(int Type, const FVector &SpawnLocation, const FRotator &SpawnRotation)
{
    ALegoCarChasis *vehicle;

    switch (Type)
    {
        case Lego_Enemy_Vehicle:

        vehicle = GetWorld()->SpawnActor<AEnemyLegoVehicle>(Lego_Enemy_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;

        case Lego_Char_Vehicle:

        vehicle = GetWorld()->SpawnActor<ALegoCarChasis>(Lego_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;

        default:

        vehicle = GetWorld()->SpawnActor<AEnemyLegoVehicle>(Lego_Enemy_Vehicle_Container, SpawnLocation + FVector(0,0,100.0f), SpawnRotation);
            return vehicle;
    }
}



void ULegoFightGameInstance::selectCurrentProductBrick(BrickType Type)
{
    Selected_Brick = Type;
    
}

void ULegoFightGameInstance::selectCurrentProductBrickColor(FLinearColor Color)
{
    Selected_Color = Color;
}
