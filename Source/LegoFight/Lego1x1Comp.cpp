// Fill out your copyright notice in the Description page of Project Settings.


#include "Lego1x1Comp.h"
#include <vector>


// Sets default values
ALego1x1Comp::ALego1x1Comp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Socket_Number_Width = 2;
    Socket_Number_Height = 2;
    Socket_Number = Socket_Number_Height * Socket_Number_Width;

    Plugin_Number_Width = 3;
    Plugin_Number_Height = 3;
    Plugin_Number = Plugin_Number_Width * Plugin_Number_Height;

    Type_ = Lego1x1_Comp;
    Sub_Type = Comp;

    FString constrait_name("Plug");
    UPhysicsConstraintComponent *constrait;

    for(int i=0; i<Socket_Number; i++)
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


    FString scene_name("scene");
    USceneComponent *scene;

    for(int i=0; i < Plugin_Number; i++)
    {

        scene_name = "Scene";
        scene_name.AppendInt(i);

        scene = CreateDefaultSubobject<USceneComponent>(FName(scene_name));
        scene->SetupAttachment(Brick);


        Plugin_Points.Push(scene);
    }



    FVector Begining_Pivot = FVector(12.5f, -12.5f , 50.7f);
    FVector Offset = FVector(0,0,-17);
    int  counter = 0;

    // place Socket Pivots
    for(int i=0; i<Socket_Number_Height; i++)
    {
        for(int k = 0; k<Socket_Number_Width; k++)
        {
            Plugin_Sockets[counter++]->SetRelativeLocation(Begining_Pivot + Offset);
            Offset.Y += (BRICK_LENGHT / 2);
        }

        Offset.X -= (BRICK_LENGHT / 2);
        Offset.Y = 0;
    }


    Begining_Pivot = FVector(25, -25.0f, 50.7f);
    Offset = FVector(0,0,0);
    counter = 0;

    // place Plugin Pivots
    for(int i=0; i<Plugin_Number_Height; i++)
    {
        for(int k = 0; k<Plugin_Number_Width; k++)
        {
            Plugin_Points[counter++]->SetRelativeLocation(Begining_Pivot + Offset);
            Offset.Y += (BRICK_LENGHT / 2);
        }

        Offset.X -= (BRICK_LENGHT / 2);
        Offset.Y = 0;
    }

    Current_Plugin_Index = 0;

}



// Called when the game starts or when spawned
void ALego1x1Comp::BeginPlay()
{
	Super::BeginPlay();

}



// Called every frame
void ALego1x1Comp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



