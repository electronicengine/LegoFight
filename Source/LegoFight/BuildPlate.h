// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/PlugInterface.h"
#include "BuildPlate.generated.h"

UCLASS()
class LEGOFIGHT_API ABuildPlate : public AActor, public IPlugInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildPlate();

    UPROPERTY(EditAnywhere)
        UStaticMeshComponent* Static_Mesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
        UStaticMesh* Mesh;

protected:

	virtual USceneComponent* CreatePluginPoint(FString Name) override;




};
