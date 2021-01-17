// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoCharacter.h"
#include "LegoFightGameInstance.h"
#include "Engine/Engine.h"
#include "LegoCarChasis.h"
#include "Kismet/GameplayStatics.h"
#include "RendererInterface.h"
#include "DrawDebugHelpers.h"

#include "Engine/World.h"



// Sets default values
ALegoCharacter::ALegoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    Aim_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Aim_Camera"));
    Aim_Camera->AttachTo(GetMesh());

    Interaction_Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interact"));
    Interaction_Component->SetupAttachment(RootComponent);

    Interaction_Component->SetCollisionProfileName(TEXT("OverlapAll"));
    Interaction_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnDelegateOverlap);

    Ghost_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ghost"));
    Ghost_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnGhostOverLap);

    Brick_Just_Plug = false;
    Ghost_Overlapped_Physics = false;
    Is_In_Car = false;
    Keeping_Bricks = false;
    Aiming_ = false;
    Tick_Count = 0;
    OffSet_Rotation.Roll = 0;
    OffSet_Rotation.Yaw = 0;
    OffSet_Rotation.Pitch = 0;

}

// Called when the game starts or when spawned
void ALegoCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ALegoCharacter::BeginPlay()"));

    UGameplayStatics::GetGameMode(GetWorld());
//    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
//    game_instance->loadLoginLevel();


//    Skeletal_Mesh->AddRadialImpulse(GetActorLocation(), 100, 1000, RIF_Constant, true);

}

// Called every frame
void ALegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(Aiming_ == true && Brick_Just_Plug == false)
    {
        FHitResult OutHit;

        FVector Start = Aim_Camera->GetComponentLocation();

        FVector forward_vector = Aim_Camera->GetComponentRotation().Vector();

        FVector End = Start + (forward_vector * 3000.0f);

        FCollisionQueryParams CollisionParams;


        Tick_Count++;
        GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

        if(Cast<ABrick>(OutHit.Actor))
        {
            if(Interactable_Brick != nullptr)
            {
                ABrick *target_brick = Cast<ABrick>(OutHit.Actor);
                Target_Brick = target_brick;
                Target_Car = nullptr;


                target_brick->highLightPlugin(Ghost_Component, Ghost_Possible_Material, Ghost_Imposible_Material,
                                              Interactable_Brick, OutHit, OffSet_Rotation);
            }
            else if(Interactable_Brick == nullptr)
            {
                Object_NearBy = Cast<ABrick>(OutHit.Actor);
            }
        }
        else if(Cast<ALegoCarChasis>(OutHit.Actor))
        {
            ALegoCarChasis *car = Cast<ALegoCarChasis>(OutHit.Actor);
            Target_Car = car;
            Target_Brick = nullptr;

            if(Interactable_Brick != nullptr)
            {
                Target_Car->highLightPlugin(Ghost_Component, Ghost_Possible_Material, Ghost_Imposible_Material,
                                              Interactable_Brick, OutHit, OffSet_Rotation);
            }

        }
        else
        {
            if(Ghost_Component != nullptr)
            {
                Ghost_Component->SetVisibility(false);
            }
            Target_Brick = nullptr;
            Target_Car = nullptr;
        }

//        DrawDebugLine(GetWorld(), Start, End, FColor::Orange, true);
//        DrawDebugBox(GetWorld(), OutHit.ImpactPoint, FVector(0, 0, 0), FColor::Green, true);

    }

}



// Called to bind functionality to input
void ALegoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ALegoCharacter::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALegoCharacter::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &ALegoCharacter::turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ALegoCharacter::lookUp);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALegoCharacter::fire);
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ALegoCharacter::aimStart);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ALegoCharacter::aimEnd);

    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ALegoCharacter::equip);
    PlayerInputComponent->BindAction("Plug", IE_Pressed, this, &ALegoCharacter::plugObject);
    PlayerInputComponent->BindAction("TurnObject", IE_Pressed, this, &ALegoCharacter::turnObject);

}

void ALegoCharacter::OnDelegateOverlap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    if(Aiming_ == false)
    {
        ALegoCarChasis *car = Cast<ALegoCarChasis>(OtherActor);

        if(car != nullptr)
            Object_NearBy = car;
        else
        {
            ABrick *brick = Cast<ABrick>(OtherActor);

            if(brick != nullptr)
                Object_NearBy = brick;
        }

    }

}

void ALegoCharacter::OnGhostOverLap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    UStaticMeshComponent *component = Cast<UStaticMeshComponent>(OtherComp);
    static int last_tick = 0;


    if(component != nullptr)
    {

//        GEngine->AddOnScreenDebugMessage(-1 ,5, FColor::Cyan, component->GetCollisionProfileName().ToString());
        if(component->GetCollisionProfileName().ToString() == "PhysicsActor")
        {
//            Ghost_Overlapped_Physics = true;
            last_tick = Tick_Count;
        }

        if(component->GetCollisionProfileName().ToString() == "OverlapAll")
        {
            if(std::abs(Tick_Count - last_tick) >= 3)
            {
//                Ghost_Overlapped_Physics = false;
            }
        }
    }
}


void ALegoCharacter::moveForward(float Value)
{
    if((Controller) && Value != 0.0f)
    {
        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);


        AddMovementInput(direction, Value);
    }
}



void ALegoCharacter::moveRight(float Value)
{
    if((Controller) && Value != 0.0f)
    {

        const FVector direction = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::Y);



        AddMovementInput(direction, Value);
    }
}



void ALegoCharacter::turn(float Value)
{
    AddControllerYawInput(Value );

}



void ALegoCharacter::lookUp(float Value)
{
    AddControllerPitchInput(Value);

}



void ALegoCharacter::fire()
{

}



void ALegoCharacter::aimStart()
{

    Camera->SetActive(false);
    Aim_Camera->SetActive(true);
    Aiming_ = true;



}



void ALegoCharacter::aimEnd()
{
    Camera->SetActive(true);
    Aim_Camera->SetActive(false);

    Aiming_ = false;
    Ghost_Component->SetVisibility(false);
    Target_Brick = nullptr;
    Target_Car = nullptr;
    Brick_Just_Plug = false;
    Tick_Count = 0;
}



void ALegoCharacter::equip()
{

    if(Object_NearBy != nullptr)
    {
        if(Cast<ALegoCarChasis>(Object_NearBy))
        {
            enteredToCar();

        }
        else if(Cast<ABrick>(Object_NearBy))
        {

            if(Interactable_Brick == Cast<ABrick>(Object_NearBy))
                dropObject(Interactable_Brick);
            else
            {
                if(Interactable_Brick == nullptr)
                {
                    Interactable_Brick = Cast<ABrick>(Object_NearBy);
                    grapObject(Interactable_Brick);

                }
                else
                {

                    dropObject(Interactable_Brick);
                    Interactable_Brick = Cast<ABrick>(Object_NearBy);

                    grapObject(Interactable_Brick);

                }
            }


        }
    }
    else
    {
        if(Interactable_Brick != nullptr)
            dropObject(Interactable_Brick);
    }

}



void ALegoCharacter::turnObject()
{
    if(OffSet_Rotation.Yaw == 360)
        OffSet_Rotation.Yaw = 0;

    OffSet_Rotation.Yaw += 90;

    Ghost_Component->AddLocalRotation(OffSet_Rotation);
}



void ALegoCharacter::grapObject(ABrick *Object)
{

    if(Object != nullptr)
    {
        Object->SetActorEnableCollision(false);

        Object->enablePhysics(false);

        Object->SetActorRotation(FRotator(0,0,0));

        Object->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
                                                                         EAttachmentRule::SnapToTarget,
                                                                         EAttachmentRule::KeepWorld, true), TEXT("grab_socket"));
        Ghost_Component->SetStaticMesh(Object->getBrickMesh());
        Keeping_Bricks = true;

    }
}



void ALegoCharacter::dropObject(ABrick *Object)
{

    if(Object != nullptr)
    {
        const FDetachmentTransformRules &attachment_rules = FDetachmentTransformRules(EDetachmentRule::KeepWorld,
                                                               EDetachmentRule::KeepWorld,
                                                               EDetachmentRule::KeepWorld, true);
        Object->DetachFromActor(attachment_rules);
        Object->enablePhysics(true);
        Object->SetActorEnableCollision(true);

        Interactable_Brick = nullptr;
        Object_NearBy = nullptr;

    }

    Keeping_Bricks = false;

}



void ALegoCharacter::plugObject()
{
    if(Interactable_Brick != nullptr && Aiming_ == true)
    {
        if(Ghost_Component->IsVisible())
        {
            if(Target_Brick != nullptr)
            {
                Target_Brick->plugTheBrick(Interactable_Brick, OffSet_Rotation);
            }
            else if(Target_Car != nullptr)
            {
                Target_Car->plugTheBrick(Interactable_Brick, OffSet_Rotation);                
            }

            Interactable_Brick = nullptr;
            Target_Car = nullptr;
            Object_NearBy = nullptr;
            Brick_Just_Plug = true;
            Keeping_Bricks = false;

        }

        Ghost_Component->SetVisibility(false);


    }
}



void ALegoCharacter::enteredToCar()
{

    if(Interactable_Brick != nullptr)
        dropObject(Interactable_Brick);

    if(Object_NearBy != nullptr)
    {
        Interactable_Car = Cast<ALegoCarChasis>(Object_NearBy);

        if(Interactable_Car != nullptr)
        {


            Interactable_Car->enterCar(this);
            Is_In_Car = true;

        }
    }



}



void ALegoCharacter::exitedFromCar()
{
    Is_In_Car =false;
    Interactable_Car = nullptr;
    Object_NearBy = nullptr;

    AddActorLocalOffset(FVector(0, -250, 0));
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    Is_In_Car = false;
}





