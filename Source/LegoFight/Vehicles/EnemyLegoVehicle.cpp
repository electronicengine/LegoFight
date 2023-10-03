// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLegoVehicle.h"
#include "Engine/Engine.h"
#include "WheeledVehicleMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "EnemyVehicleAIController.h"



AEnemyLegoVehicle::AEnemyLegoVehicle()
{
    CrashedTo_Target = false;

    Team_Id = 0;

    Overlap_Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap_Box"));
    Overlap_Box->SetupAttachment(RootComponent);
    Overlap_Box->SetCollisionProfileName(TEXT("OverlapAll"));
    Overlap_Box->SetGenerateOverlapEvents(true);
    Overlap_Box->SetWorldScale3D(FVector(11.0, 9.33, 1.0));

    Overlap_Box->OnComponentBeginOverlap.AddDynamic(this, &AEnemyLegoVehicle::OnDelegateOverlap);
    //static ConstructorHelpers::FObjectFinder<UBlueprint> AICon(TEXT("Blueprint'/Game/bricks/test_ai.test_ai'"));
    //AIControllerClass = AICon.Object->GeneratedClass;

    AIControllerClass = AEnemyVehicleAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}



void AEnemyLegoVehicle::BeginPlay()
{
    Super::BeginPlay();

 
    
}

void AEnemyLegoVehicle::GetActorEyesViewPoint(FVector& Location, FRotator& Rotation) const
{
    Location = GetMesh()->GetSocketLocation("EYE");
    Rotation = GetMesh()->GetSocketRotation("EYE");
}





void AEnemyLegoVehicle::OnDelegateOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    ALegoCarChasis *user_car = Cast<ALegoCarChasis>(OtherActor);

    if(user_car != nullptr)
    {
        if (user_car->Team_Id == 1) {
            CrashedTo_Target = true;
        }
    }
}


void AEnemyLegoVehicle::fireToEnemy(FRotator &ToEnemyRot)
{

    static int last = 0; 
    int now = time(NULL);

    static int weapon_index;

    if (now - last >= 1) {

        if (Weapons.size() != 0)
        {

            for(std::pair<int, AWeapon*> weapon : Weapons) {
                weapon.second->aimToRotation(ToEnemyRot);
                weapon.second->fire();

            }
        }

        last = now;

    }
}

