// Fill out your copyright notice in the Description page of Project Settings.


#include "LegoCharacter.h"
#include "Engine/Engine.h"
#include "../Vehicles/LegoCarChasis.h"
#include "../LegoFightGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "RendererInterface.h"
#include "DrawDebugHelpers.h"
#include "../LegoFightSaveGame.h"
#include "Json.h"

#include "Engine/World.h"



// Sets default values
void ALegoCharacter::setupMesh()
{

    static ConstructorHelpers::FObjectFinder<USkeletalMesh>
            mesh_asset(TEXT("SkeletalMesh'/Game/characters/skeleto/lego_man.lego_man'"));
    USkeletalMesh* mesh = mesh_asset.Object;
    GetMesh()->SetSkeletalMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset0(TEXT("Material'/Game/characters/materials/Head.Head'"));
    UMaterial *head_material = material_asset0.Object;
    GetMesh()->SetMaterial(0, head_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset1(TEXT("Material'/Game/characters/materials/UnderPants.UnderPants'"));
    UMaterial *underpants_material = material_asset1.Object;
    GetMesh()->SetMaterial(1, underpants_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset2(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial *hand_material = material_asset2.Object;
    GetMesh()->SetMaterial(2, hand_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset3(TEXT("Material'/Game/characters/materials/Arms.Arms'"));
    UMaterial *arm_material = material_asset3.Object;
    GetMesh()->SetMaterial(3, arm_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset4(TEXT("Material'/Game/characters/materials/Waists.Waists'"));
    UMaterial *waist_material = material_asset4.Object;
    GetMesh()->SetMaterial(4, waist_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            material_asset5(TEXT("Material'/Game/characters/materials/Legs.Legs'"));
    UMaterial *leg_material = material_asset5.Object;
    GetMesh()->SetMaterial(5, leg_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
            ghost_asset(TEXT("Material'/Game/bricks/materials/ghost_possible.ghost_possible'"));
    Ghost_Possible_Material = ghost_asset.Object;

    static ConstructorHelpers::FObjectFinder<UMaterial>
            ghost_impossible_asset(TEXT("Material'/Game/bricks/materials/ghost_impossible.ghost_impossible'"));
    Ghost_Imposible_Material = ghost_impossible_asset.Object;

    //static ConstructorHelpers::FClassFinder<ADestrictable>
    //         bullet(TEXT("Class'/Script/LegoFight.Bullet_C'"));
    Bullet_Container = ABullet::StaticClass();
    Offset_Step = 0;
}



ALegoCharacter::ALegoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    setupMesh();


    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);


    Aim_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Aim_Camera"));
    Aim_Camera->AttachTo(GetMesh());    

    Barrel_ = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
    Barrel_->SetupAttachment(GetMesh());

    Interaction_Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interact"));
    Interaction_Component->SetupAttachment(RootComponent);

    Interaction_Component->SetCollisionProfileName(TEXT("OverlapAll"));
    Interaction_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnDelegateOverlap);

    Ghost_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ghost"));
    Ghost_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnGhostOverLap);

    Brick_Plugable = true;
    Brick_Just_Plug = false;
    Is_In_Car = false;
    Keeping_Bricks = false;
    Aiming_ = false;
    Tick_Count = 0;
    OffSet_Rotation.Roll = 0;
    OffSet_Rotation.Yaw = 0;
    OffSet_Rotation.Pitch = 0;
    OffSet_Location = FVector(0, 0, 0);

}

// Called when the game starts or when spawned
void ALegoCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ALegoCharacter::BeginPlay()"));

    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->loadCharacterPanel();
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

        Aim_Impact_Point = OutHit.ImpactPoint;

        if(Cast<ABrick>(OutHit.Actor))
        {
            if(Grabbable_Brick != nullptr)
            {
                ABrick *target_brick = Cast<ABrick>(OutHit.Actor);

                if (Target_Brick != target_brick) {
                    OffSet_Location = FVector(0, 0, 0);
                    OffSet_Rotation = FRotator(0, 0, 0);
                }
                
                Target_Brick = target_brick;
                Target_Car = nullptr;

                Brick_Plugable = target_brick->highLightPlugin(Ghost_Component, Ghost_Possible_Material, Ghost_Imposible_Material,
                                              Grabbable_Brick, OutHit, OffSet_Rotation, OffSet_Location, Ghost_Overlapped_Brick);
            }
            else if(Grabbable_Brick == nullptr)
            {
                Object_NearBy = Cast<ABrick>(OutHit.Actor);
            }
        }
        else if(Cast<ALegoCarChasis>(OutHit.Actor))
        {
            ALegoCarChasis *car = Cast<ALegoCarChasis>(OutHit.Actor);
            Target_Car = car;
            Target_Brick = nullptr;

            if(Grabbable_Brick != nullptr)
            {
                Brick_Plugable = Target_Car->highLightPlugin(Ghost_Component, Ghost_Possible_Material, Ghost_Imposible_Material,
                                              Grabbable_Brick, OutHit, OffSet_Rotation, OffSet_Location, Ghost_Overlapped_Brick);
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
    //PlayerInputComponent->BindAction("Aim", IE_Released, this, &ALegoCharacter::aimEnd);

    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ALegoCharacter::openInventoryWidget);
    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ALegoCharacter::equip);
    PlayerInputComponent->BindAction("OffSetItem", IE_Pressed, this, &ALegoCharacter::offSetItem);

    PlayerInputComponent->BindAction("Plug", IE_Pressed, this, &ALegoCharacter::plugObject);
    PlayerInputComponent->BindAction("TurnObject", IE_Pressed, this, &ALegoCharacter::turnObject);
    PlayerInputComponent->BindAction("Buy", IE_Pressed, this, &ALegoCharacter::buyBrick);
    PlayerInputComponent->BindAction("Save", IE_Pressed, this, &ALegoCharacter::save);
    PlayerInputComponent->BindAction("Load", IE_Pressed, this, &ALegoCharacter::load);

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

          if (brick != nullptr) {

              if (brick->Owner_Car != nullptr) {
                  Object_NearBy = brick->Owner_Car;
              }
              else {
                  Object_NearBy = brick;
              }

            }
        }

    }

}

void ALegoCharacter::OnGhostOverLap(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    ABrick *overlapped_brick = Cast<ABrick>(OtherActor);

    if(overlapped_brick != nullptr)
    {
        Ghost_Overlapped_Brick = overlapped_brick;
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


    ABullet *bullet_ptr;
    FVector spawn_location = Aim_Camera->GetComponentLocation();
    FRotator spawn_rotation = Aim_Camera->GetForwardVector().Rotation();

    bullet_ptr = GetWorld()->SpawnActor<ABullet>(Bullet_Container,  spawn_location, spawn_rotation);

    if(bullet_ptr != nullptr)
        bullet_ptr->addFireImpulse(Aim_Camera->GetForwardVector().Rotation().Vector(), 30000);
}



void ALegoCharacter::aimStart()
{
    if (Aiming_) {
        aimEnd();
    }
    else {
        Camera->SetActive(false);
        Aim_Camera->SetActive(true);
        Aiming_ = true;

    }

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


    if (Cast<ALegoCarChasis>(Object_NearBy))
    {
        enteredToCar();

    }else if(Cast<ABrick>(Object_NearBy))
    {

        if(Grabbable_Brick == Cast<ABrick>(Object_NearBy))
            dropObject(Grabbable_Brick);
        else
        {
            if(Grabbable_Brick == nullptr)
            {
                Grabbable_Brick = Cast<ABrick>(Object_NearBy);
                grapObject(Grabbable_Brick);

            }
            else
            {

                dropObject(Grabbable_Brick);
                Grabbable_Brick = Cast<ABrick>(Object_NearBy);

                grapObject(Grabbable_Brick);

            }
        }


    }else
    {
        if(Grabbable_Brick != nullptr)
            dropObject(Grabbable_Brick);

    }

}

void ALegoCharacter::offSetItem()
{


    int offset = 0;

    if (Ghost_Component) {
        Offset_Step += 1;

        if(Pivot_Width != 0)
            offset = Offset_Step % Pivot_Width;
        else 
            offset = 0;
        

        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString::FromInt(Pivot_Width));
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::FromInt(Offset_Step));
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(offset));


        if (Ghost_Component->IsVisible()) {
            OffSet_Location -= FVector(0, 25, 0);

        }


        if (offset == 0) {
            OffSet_Location = FVector(0, 0, 0);
            Offset_Step = 0;
        }


    }
}



void ALegoCharacter::interact()
{
    if(Object_NearBy != nullptr)
    {

        if(Cast<AWeapon>(Object_NearBy))
        {

        }
    }
}



void ALegoCharacter::turnObject()
{
    if (OffSet_Location == FVector(0, 0, 0)) {
        if (OffSet_Rotation.Yaw == 360)
            OffSet_Rotation.Yaw = 0;

        OffSet_Rotation.Yaw += 90;
    }

}



void ALegoCharacter::grapObject(ABrick *Object)
{

    if(Object != nullptr)
    {


        Object->enablePhysics(false);
        Object->setCollisionProfile("OverlapAll");

        Object->SetActorRotation(FRotator(0,0,0));

        Object->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
                                                                         EAttachmentRule::SnapToTarget,
                                                                         EAttachmentRule::KeepWorld, true), TEXT("grab_socket"));
        Ghost_Component->SetStaticMesh(Object->getBrickMesh());
        Keeping_Bricks = true;

        FVector Origin;
        FVector BoxExtent;
        float SphereRadius;

        UKismetSystemLibrary::GetComponentBounds(Ghost_Component, Origin, BoxExtent, SphereRadius);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, BoxExtent.ToString());

        if(BoxExtent.Y >= BoxExtent.X)
            Pivot_Width = (BoxExtent.Y / 12.5);
        else
            Pivot_Width = (BoxExtent.X / 12.5);



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
        Object->setCollisionProfile("BlockAll");
//        Object->SetActorEnableCollision(true);

        Grabbable_Brick = nullptr;
        Object_NearBy = nullptr;

    }

    Keeping_Bricks = false;

}



void ALegoCharacter::plugObject()
{
    if(Grabbable_Brick != nullptr && Aiming_ == true && Brick_Plugable == true)
    {
        if(Ghost_Component->IsVisible())
        {

            if(Target_Brick != nullptr)
            {

                Target_Brick->plugTheBrick(Grabbable_Brick, -1, OffSet_Rotation, OffSet_Location);
            }
            else if(Target_Car != nullptr)
            {
                Target_Car->plugTheBrick(Grabbable_Brick, -1, OffSet_Rotation, OffSet_Location);
            }

            Grabbable_Brick = nullptr;
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

    if(Grabbable_Brick != nullptr)
        dropObject(Grabbable_Brick);

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



void ALegoCharacter::useWeapon()
{

}



void ALegoCharacter::openInventoryWidget()
{

    ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->loadInvantoryPanel();

}

void ALegoCharacter::buyBrick()
{

    if (Grabbable_Brick == nullptr) {
        ULegoFightGameInstance* game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
        Grabbable_Brick = Cast<ABrick>(game_instance->spawnItem(FVector(0, 0, 0), FRotator(0, 0, 0)));

        if (Grabbable_Brick != nullptr)
        {

            grapObject(Grabbable_Brick);
            Brick_Just_Plug = false;
        }
    }


}



void ALegoCharacter::save()
{


    ALegoCarChasis *vehicle = Cast<ALegoCarChasis>(Target_Car);
    ULegoFightGameInstance* game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    game_instance->savePanel(vehicle, Aim_Impact_Point);



}


void ALegoCharacter::load()
{

//    construction_info = save_game->Construction_Info;
    //ULegoFightGameInstance *game_instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());

    //ULegoFightSaveGame *save_game = Cast<ULegoFightSaveGame>(UGameplayStatics::CreateSaveGameObject(ULegoFightSaveGame::StaticClass()));
    //save_game =  Cast<ULegoFightSaveGame>(UGameplayStatics::LoadGameFromSlot(FString("myslot"), 0));

    //TSharedPtr<FJsonObject> de_json = save_game->deserializeJsonObject(save_game->SaveObject);
    //ConstructionInfo de_info = save_game->convertJsonToConstructionInfo(de_json);

    //IBuiltInInterface::buildFromConstructionInfo(de_info, Aim_Impact_Point, game_instance);

    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("loaded"));




}





