// Fill out your copyright notice in the Description page of Project Settings.


#include "SlingInterface.h"
#include "../Brick.h"
#include "BuilderInterface.h"
// Include necessary UE4 headers
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
// Add default functionality here for any ISlingInterface functions that are not pure virtual.

ISlingInterface::ISlingInterface()
{
	Sling_Mode_Enable = false;
	Stretching_Sling = false;
}



void ISlingInterface::throwTheItem(const FVector& ForceVector, AActor* Item)
{
    if (Cast<ABrick>(Item)) {

        Cast<IBuilderInterface>(this)->dropObject(Cast<ABrick>(Item));

        Cast<ABrick>(Item)->Brick->AddImpulse(ForceVector * 80);

    }
}

void ISlingInterface::strechTheSling() {

       if (Sling_Mode_Enable) {

       FVector2D MousePosition;

       if (Viewport_Client && Projectile_Item) {
           Viewport_Client->GetMousePosition(MousePosition);


           if (Stretching_Sling) {
               Stretching_Sling = false;
               Stretch_End = FVector(MousePosition.X, MousePosition.Y, 0);

               throwTheItem(Throw_Force, Projectile_Item);
               Projectile_Item = nullptr;
           }
           else {
               Stretching_Sling = true;
               Stretch_Begin = FVector(MousePosition.X, MousePosition.Y, 0);
           }
       }
 
    }
}

// ...

