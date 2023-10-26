// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Brick.h"
#include "Interfaces/BuilderInterface.h"
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
		UStaticMeshComponent* AttachedComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsConstraintComponent* Constrait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsConstraintComponent* AttachPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Machine_Running;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Cable_Hook;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ApplyForce_ToAdd;

	UFUNCTION(BlueprintCallable)
		void setHeightOffset(int Offset);


	UFUNCTION(BlueprintImplementableEvent, Category = "Constrait")
		void setupConstraits();

	UFUNCTION(BlueprintImplementableEvent, Category = "Hook")
		void hookCableToItem(UStaticMeshComponent *Hook);

	UFUNCTION(BlueprintCallable)
		UStaticMeshComponent* getBrickComponent() { return Brick; };


	bool turnOnOffMachine();
	void setGhostComponent(UStaticMesh* Mesh);
	virtual void BeginPlay() override;
	virtual void setMaterialColor(FLinearColor Color);
	

};
