// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyVehicleAIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Math/Vector.h"
#include "Engine/Engine.h"
#include "LegoCarChasis.h"



AEnemyVehicleAIController::AEnemyVehicleAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    setupPerception();

    Enemy_Found = false;
    Current_Task_Index = 0;


}



void AEnemyVehicleAIController::setupPerception()
{

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRadius;
    SightConfig->LoseSightRadius = AILoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyVehicleAIController::OnPerceptionUpdated);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);

}



void AEnemyVehicleAIController::OnPossess(APawn *Pawn)
{
    Super::OnPossess(Pawn);
}



void AEnemyVehicleAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

//    Task_Sequence_Attack[Current_Task_Index];

}



int AEnemyVehicleAIController::attackToUser()
{

    Owner_Pawn = Cast<AEnemyLegoVehicle>(GetPawn());

    if(Owner_Pawn != nullptr)
    {
        Owner_Pawn->moveLocation(Enemy_Location);
    }

    return 0;

}



int AEnemyVehicleAIController::goRandomLocation()
{
    return 0;
}



int AEnemyVehicleAIController::turnArround()
{
    return 0;
}



void AEnemyVehicleAIController::BeginPlay()
{
    Super::BeginPlay();

    Owner_Pawn = Cast<AEnemyLegoVehicle>(GetPawn());

    if (GetPerceptionComponent() != nullptr)
    {

//        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("AICube Controller Set"));
    }
    else
    {
//        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Some Problem Occured"));

    }

}



void AEnemyVehicleAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{

    int enemy_number_nearby = 0;

    for(int i = 0; i < UpdatedActors.Num(); i++)
    {
        TargetVehicle = Cast<ALegoCarChasis>(UpdatedActors[i]);

        if(TargetVehicle != nullptr)
        {

            if(TargetVehicle->Team_Id == 1)
            {

                enemy_number_nearby++;
                Enemy_Found = true;
                Enemy_Location = TargetVehicle->GetActorLocation();

                if(TargetVehicle->carHasPassenger())
                    attackToUser();
                
            }

        }
    }

    if(enemy_number_nearby == 0)
        Enemy_Found = false;

}


