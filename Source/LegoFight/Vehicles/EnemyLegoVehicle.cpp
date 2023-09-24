// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLegoVehicle.h"
#include "Engine/Engine.h"
#include "WheeledVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EnemyVehicleAIController.h"



AEnemyLegoVehicle::AEnemyLegoVehicle()
{
    Crash_Target = false;

    Team_Id = 0;

    Overlap_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap_Box"));
    Overlap_Box->SetupAttachment(RootComponent);
    Overlap_Box->SetCollisionProfileName(TEXT("OverlapAll"));
    Overlap_Box->SetGenerateOverlapEvents(true);
    Overlap_Box->SetWorldScale3D(FVector(11.0, 9.33, 1.0));

    Overlap_Box->OnComponentBeginOverlap.AddDynamic(this, &AEnemyLegoVehicle::OnDelegateOverlap);

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AEnemyVehicleAIController::StaticClass();

}



void AEnemyLegoVehicle::BeginPlay()
{
    Super::BeginPlay();

    FVector spawn_loc = GetActorLocation() + FVector(150, 0, 0);

    //Path_Finder = GetWorld()->SpawnActor<APathFinder>(APathFinder::StaticClass(), spawn_loc, FRotator(0, 0, 0));
    
    
}



void AEnemyLegoVehicle::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
    //followThePathFinder();

}



void AEnemyLegoVehicle::moveLocation(const FVector &Location)
{
    followThePathFinder(Location);
    //AAIController *path_finder_controller = Cast<AAIController>(Path_Finder->GetController());

    //if(path_finder_controller != nullptr)
    //{
    //    fire();
    //    path_finder_controller->MoveToLocation(Location);
    //}

}



void AEnemyLegoVehicle::OnDelegateOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    ALegoCarChasis *user_car = Cast<ALegoCarChasis>(OtherActor);

    if(user_car != nullptr)
    {
        if(user_car->Team_Id == 1)
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("crashed user"));
    }
}



void AEnemyLegoVehicle::followThePathFinder(const FVector& Location)
{

    static FRotator error_rot;
    static FVector error_dest;
    static float stop_distance = 100;

    Destination_Location = Location;
    GetVehicleMovement()->SetHandbrakeInput(false);

    DrawDebugLine(GetWorld(), GetActorLocation(), Destination_Location, FColor::Red);

    error_rot = (UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Destination_Location) - GetActorRotation());
    error_dest = Destination_Location - GetActorLocation();



    if((error_rot.Yaw < -95))
    {
        moveForward(-1.0f);
        moveRight(1.0f);
    }
    else if((error_rot.Yaw > 95))
    {

        moveForward(-1.0f);
        moveRight(-1.0f);

    }
    else if((error_rot.Yaw <= 30.0f && error_rot.Yaw >= -30.0f))
    {
        moveForward(1.0f);

        moveRight(0);
    }
    else
    {
        moveForward(1.0f);

        if(error_rot.Yaw < 0)
            moveRight(-1.0f);
        else
            moveRight(1.0f);
    }


    if(error_dest.Size() <= stop_distance)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::SanitizeFloat(stop_distance));

        if(Crash_Target)
        {
            moveForward(1);

        }
        else
        {
            moveForward(0);
            GetVehicleMovement()->SetHandbrakeInput(true);
        }

    }
    else
    {
        //stop_distance = (GetVehicleMovement()->GetEngineRotationSpeed()) / 3.3;
    }

    if(error_dest.Size() <= 100)
    {

        if(Crash_Target)
        {
            moveForward(1);

        }
        else
        {
            moveForward(0);
            GetVehicleMovement()->SetHandbrakeInput(true);
        }

    }


}
