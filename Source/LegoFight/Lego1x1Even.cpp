// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego1x1Even.h"


// Sets default values
ALego1x1Even::ALego1x1Even()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

//    Brick_Number_Width = 1;
//    Brick_Number_Height = 2;
//    Brick_Number = Brick_Number_Height * Brick_Number_Width;

//    Plugin_Number_Width = 3;
//    Plugin_Number_Height = 3;
//    Plugin_Number = Plugin_Number_Width * Plugin_Number_Height;

    Type_ = Lego1x1_Even;
    Sub_Type = Comp;

    FString constrait_name("Plug");
    UPhysicsConstraintComponent *constrait;

    for(int i=0; i<2; i++)
    {
        constrait_name = "Plug";
        constrait_name.AppendInt(i);

        constrait = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(constrait_name));
        constrait->SetupAttachment(Brick);
        constrait->SetDisableCollision(true);

        constrait->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
        constrait->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
        constrait->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);

        Plugin_Sockets.Push(constrait);
    }

}

// Called when the game starts or when spawned
void ALego1x1Even::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void ALego1x1Even::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

