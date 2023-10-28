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

        float Mass = Cast<ABrick>(Item)->Mass_; // Get the mass of the projectile
        FVector InitialVelocityVector = ForceVector  * (100 / Mass); // Calculate the initial velocity vector

        FVector CurrentVelocity = FVector(0,0,0); // Get the current velocity

        FVector DeltaVelocity = InitialVelocityVector - CurrentVelocity; // Calculate the change in velocity

        FVector ImpulseForce = Mass * DeltaVelocity; // Calculate the impulse force

        // Apply the impulse force to the projectile

        Cast<IBuilderInterface>(this)->dropObject(Cast<ABrick>(Item));


        Cast<ABrick>(Item)->Brick->AddImpulse(ImpulseForce);
        FVector TorqueForce = FVector(0.0f, 1000.0f, 0);  // Adjust the torque force as needed

   // Apply the torque force for local rotation
        Cast<ABrick>(Item)->Brick->AddTorqueInRadians(TorqueForce, NAME_None, true);
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

