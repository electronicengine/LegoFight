// Fill out your copyright notice in the Description page of Project Settings.


#include "Destrictable.h"
#include "Brick.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "LegoFightGameInstance.h"

// Sets default values
ADestrictable::ADestrictable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InitialLifeSpan = 2;
	Default_Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(Default_Root);
    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    Material = material_asset.Object;

    for (int i = 0; i < 100; i++) {
        FName name = FName(FString("Piece_") + FString::FromInt(i));
        UStaticMeshComponent* one_piece = CreateDefaultSubobject<UStaticMeshComponent>(name);
        one_piece->SetSimulatePhysics(true);
        one_piece->SetCollisionProfileName(FName("BlockAll"));
        one_piece->SetupAttachment(RootComponent);

        Pieces.Push(one_piece);

    }
}

// Called when the game starts or when spawned
void ADestrictable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestrictable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestrictable::setColor(FVector ColorVec)
{
	for (int i = 0; i < Pieces.Num(); i++) {
		Pieces[i]->CreateDynamicMaterialInstance(0);
		Pieces[i]->SetVectorParameterValueOnMaterials(FName("BaseColor"), FVector(ColorVec));
	}


}

void ADestrictable::setMesh(FString Name)
{
    ULegoFightGameInstance *instance = Cast<ULegoFightGameInstance>(GetGameInstance());
    TArray<UStaticMesh*> assets = instance->Destructables[Name];

    int i = 0;
    for (UStaticMesh* asset : assets)
    {
        GEngine->AddOnScreenDebugMessage(-1, -5, FColor::Orange, asset->GetName());

        UStaticMesh* mesh = Cast<UStaticMesh>(asset);

        Pieces[i]->SetStaticMesh(mesh);
        Pieces[i++]->SetMaterial(0, Material);
    }
}


