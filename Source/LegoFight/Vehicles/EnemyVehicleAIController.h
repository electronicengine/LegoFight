// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "EnemyLegoVehicle.h"
#include <functional>
#include <vector>
#include "EnemyVehicleAIController.generated.h"

/**
 * 
 */
UCLASS()
class LEGOFIGHT_API AEnemyVehicleAIController : public AAIController
{
	GENERATED_BODY()
	
private:


    UPROPERTY(EditAnywhere, Category = AI)
    float AISightRadius = 16000.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AISightAge = 20.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AILoseSightRadius = 300.0f;

    UPROPERTY(EditAnywhere, Category = AI)
    float AIFieldOfView = 180.0f;

    UPROPERTY(VisibleAnywhere, Category = AI)
    class UAISenseConfig_Sight* SightConfig;

    void setupPerception();

    FVector Enemy_Location;
    ALegoCarChasis *TargetVehicle;

//    std::vector<std::function<int()>()>Task_Sequence_Patrol;
//    std::vector<std::function<int()>()>Task_Sequence_Attack;
    int Current_Task_Index;
    int Enemy_Found;
    APawn* OwnerChar;

    std::list<std::function<bool()>> Jobs;

protected:
    virtual void OnPossess(APawn* InPawn) override;

public:

    AEnemyVehicleAIController();

    virtual void BeginPlay() override;


    virtual void Tick(float DeltaSeconds) override;

    bool crashToEnemy();
    int attackToUser();
    int goRandomLocation();
    int turnArround();
    bool IsPointReached(const FVector& Point, const TArray<FVector>& ReachedPoints);
    bool moveTo(const FVector& Target);

    TQueue<FVector> PathPointQueue;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	
};
