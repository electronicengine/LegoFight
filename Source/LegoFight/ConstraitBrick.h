// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Brick.h"
#include "ConstraitBrick.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AConstraitBrick : public ABrick
{
	GENERATED_BODY()



public:
	AConstraitBrick();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Add;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Machine_Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Cable_Hook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Cable_Start_Hooked;

	UFUNCTION(BlueprintCallable)
		void setHeightOffset(int Offset);

	UFUNCTION(BlueprintImplementableEvent, Category = "Hook")
		void hookCableToItem(AConstraitBrick *Hook);

	UFUNCTION(BlueprintCallable)
		UStaticMeshComponent* getBrickComponent() { return Brick; };

	bool turnOnOffMachine();
	void setGhostComponent(UStaticMesh* Mesh);
	virtual void BeginPlay() override;
	virtual void setMaterialColor(FLinearColor Color);

};
