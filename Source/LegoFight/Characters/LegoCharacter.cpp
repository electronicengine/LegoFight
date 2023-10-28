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
#include "../Widgets/CharacterWidget.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"


ALegoCharacter::ALegoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    setupMesh();


    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
    Camera->SetRelativeLocation(FVector(-165.196976, 0, 192.280701));
    Camera->SetRelativeRotation(FRotator(0, -10.0, 0));

    Builder_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Aim_Camera"));
    Builder_Camera->AttachTo(GetMesh());
    Builder_Camera->SetRelativeLocation(FVector(-66.029541, -134.599854, 759.365051));
    Builder_Camera->SetRelativeRotation(FRotator(0.000003, -20.000061, 90.000221));
    Builder_Camera->bUsePawnControlRotation = true;


    Sling_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Sling_Camera"));
    Sling_Camera->AttachTo(GetMesh());
    Sling_Camera->SetRelativeLocation(FVector(-66.029541, -134.599854, 759.365051));
    Sling_Camera->SetRelativeRotation(FRotator(0.000003, -20.000061, 90.000221));

    Barrel_ = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel"));
    Barrel_->SetupAttachment(GetMesh());

    Interaction_Component = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interact"));
    Interaction_Component->SetupAttachment(RootComponent);

    Interaction_Component->SetCollisionProfileName(TEXT("OverlapAll"));


    Ghost_Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ghost"));
    //Ghost_Item->SetupAttachment(RootComponent);
    Ghost_Item->SetSimulatePhysics(false);
    Ghost_Item->SetCollisionProfileName("OverlapAll");

    Ghost = Ghost_Item;
    Camera_Mode = CameraMode::normal;

}



// Sets default values
void ALegoCharacter::setupMesh()
{

    static ConstructorHelpers::FObjectFinder<USkeletalMesh>
        mesh_asset(TEXT("SkeletalMesh'/Game/characters/skeleto/lego_man.lego_man'"));
    USkeletalMesh* mesh = mesh_asset.Object;
    GetMesh()->SetSkeletalMesh(mesh);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset0(TEXT("Material'/Game/characters/materials/Head.Head'"));
    UMaterial* head_material = material_asset0.Object;
    GetMesh()->SetMaterial(0, head_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset1(TEXT("Material'/Game/characters/materials/UnderPants.UnderPants'"));
    UMaterial* underpants_material = material_asset1.Object;
    GetMesh()->SetMaterial(1, underpants_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset2(TEXT("Material'/Game/bricks/materials/brick_material.brick_material'"));
    UMaterial* hand_material = material_asset2.Object;
    GetMesh()->SetMaterial(2, hand_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset3(TEXT("Material'/Game/characters/materials/Arms.Arms'"));
    UMaterial* arm_material = material_asset3.Object;
    GetMesh()->SetMaterial(3, arm_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset4(TEXT("Material'/Game/characters/materials/Waists.Waists'"));
    UMaterial* waist_material = material_asset4.Object;
    GetMesh()->SetMaterial(4, waist_material);

    static ConstructorHelpers::FObjectFinder<UMaterial>
        material_asset5(TEXT("Material'/Game/characters/materials/Legs.Legs'"));
    UMaterial* leg_material = material_asset5.Object;
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

    //GetMesh()->SetRelativeScale3D(FVector(0.5225, 0.5225, 0.5225));
}


// Called when the game starts or when spawned
void ALegoCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ALegoCharacter::BeginPlay()"));

    Game_Instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    Game_Instance->loadCharacterPanel();
//    Skeletal_Mesh->AddRadialImpulse(GetActorLocation(), 100, 1000, RIF_Constant, true);
    Ghost_Item->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnGhostOverLap);
    Viewport_Client = GetWorld()->GetGameViewport();

    Interaction_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnInteractBegin);
    Interaction_Component->OnComponentEndOverlap.AddDynamic(this, &ALegoCharacter::OnInteractEnd);
}

// Function to calculate and visualize projectile path
void ALegoCharacter::calculateProjectilePath(AActor* Projectile, const FVector& ForwardVector)
{
    // Set up parameters for PredictProjectilePath
    FHitResult HitResult;
    TArray<FVector> PathPositions;
    FVector LastTraceDestination;
    float k = 0;

        
    if (Cast<ABrick>(Projectile))
        k =  100 / Cast<ABrick>(Projectile)->Mass_;

    // Initial projectile parameters
    FVector StartLocation = Projectile->GetActorLocation();
    FVector LaunchVelocity = ForwardVector * k; // Adjust as needed
    float ProjectileRadius = 5;  // Adjust based on your projectile's collision size

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    // Add object types that you want to trace against (e.g., WorldStatic, WorldDynamic, etc.)

    TArray<AActor*> ActorsToIgnore;
    // Add actors that you want to ignore during the trace (if any)

    // Call PredictProjectilePath
    bool bHit = UGameplayStatics::PredictProjectilePath(
        this,
        HitResult,
        PathPositions,
        LastTraceDestination,
        StartLocation,
        LaunchVelocity,
        true,  // bTracePath
        ProjectileRadius,
        ObjectTypes,
        true,  // bTraceComplex
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame,  // DrawDebugType
        1.0f,  // DrawDebugTime
        10.0f,  // SimFrequency
        1.0f,  // MaxSimTime
        -980.0f  // OverrideGravityZ (adjust as needed, -980.0f is default gravity)
    );

    //// Process the results
    //if (bHit)
    //{
    //    // Loop through the path points and do something with them (e.g., visualize the path)
    //    for (FVector PointLocation : PathPositions)
    //    {
    //        // Visualize or do something with the path points...
    //    }
    //}
    //else
    //{
    //    // Handle the case when no hit is predicted
    //}
}




void ALegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Builder_Mode_Enable == true)
    {
        FHitResult OutHit;
        FVector Start = Builder_Camera->GetComponentLocation();
        FVector forward_vector = Builder_Camera->GetComponentRotation().Vector();
        FVector End = Start + (forward_vector * 3000.0f);
        FCollisionQueryParams CollisionParams;

        GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
        Aim_Impact_Point = OutHit.ImpactPoint;

        lookForBuildingSpace(OutHit.Actor.Get(), OutHit);
    }
    else if (Sling_Mode_Enable && Stretching_Sling) {
        FVector forward;

        UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();

        FVector2D MousePosition;
        ViewportClient->GetMousePosition(MousePosition);
            

        FVector cam_rot_vec = Sling_Camera->GetComponentRotation().Vector();

        float x = (Stretch_Begin.X - MousePosition.X) * (-cam_rot_vec.Y);
        float y = (Stretch_Begin.X - MousePosition.X) * (cam_rot_vec.X);
        float z = (MousePosition.Y - Stretch_Begin.Y);
        Throw_Force = FVector(x, y, z);
        if (Grabbable_Brick) {          
            calculateProjectilePath(Grabbable_Brick, Throw_Force);

        }
    }

}


void ALegoCharacter::OnTouchPressed(ETouchIndex::Type FingerIndex, FVector Location)
{
    Stretch_Begin = Location;
    Stretching_Sling = true;

}


void ALegoCharacter::OnTouchReleased(ETouchIndex::Type FingerIndex, FVector Location)
{
    Stretch_End = Location;
    Stretching_Sling = false;

    throwTheItem(Throw_Force, Grabbable_Brick);
    Projectile_Item = nullptr;
}

void ALegoCharacter::TouchMoved(ETouchIndex::Type FingerIndex, FVector Location)
{
    Stretch_End = Location;
    Stretching_Sling = true;
}


void ALegoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &ALegoCharacter::moveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALegoCharacter::moveRight);

    PlayerInputComponent->BindAxis("Turn", this, &ALegoCharacter::turn);
    PlayerInputComponent->BindAxis("LookUp", this, &ALegoCharacter::lookUp);
    PlayerInputComponent->BindAxis("Zoom", this, &ALegoCharacter::zoom);


    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALegoCharacter::fire);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ISlingInterface::strechTheSling);

    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ALegoCharacter::changeCameraMode);
    //PlayerInputComponent->BindAction("Aim", IE_Released, this, &ALegoCharacter::aimEnd);

    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &IInteractInterface::openInventoryWidget);
    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &IInteractInterface::equip);
    PlayerInputComponent->BindAction("OffSetItem", IE_Pressed, this, &IBuilderInterface::giveOffsetLocation);
    PlayerInputComponent->BindAction("TurnObject", IE_Pressed, this, &IBuilderInterface::giveOffsetRotation);

    PlayerInputComponent->BindAction("Plug", IE_Pressed, this, &IBuilderInterface::plugObject);
    PlayerInputComponent->BindAction("Buy", IE_Pressed, this, &IInteractInterface::buyBrick);
    PlayerInputComponent->BindAction("Save", IE_Pressed, this, &IInteractInterface::saveAndLoad);
    PlayerInputComponent->BindAction("Load", IE_Pressed, this, &ALegoCharacter::saveAndLoad);

    PlayerInputComponent->BindTouch(IE_Pressed, this, &ALegoCharacter::OnTouchPressed);
    PlayerInputComponent->BindTouch(IE_Released, this, &ALegoCharacter::OnTouchReleased);   
    PlayerInputComponent->BindTouch(IE_Repeat, this, &ALegoCharacter::TouchMoved);


}



void ALegoCharacter::OnInteractBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    if (Builder_Mode_Enable == false && Cast<IPlugInterface>(OtherActor) && !Cast<ALegoCharacter>(OtherActor)){
        interactNearby(OtherActor);
        Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setInteractButtonVisibilty(ESlateVisibility::Visible);
    }

}

void ALegoCharacter::OnInteractEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if(!Grabbable_Brick)
        Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setInteractButtonVisibilty(ESlateVisibility::Hidden);

}

void ALegoCharacter::OnGhostOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    checkGhostItemOverlap(OtherActor);
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
    if (Sling_Mode_Enable) {

        Sling_Camera->AddLocalOffset(FVector(0, Value * 10, 0));
    }
    else {
        AddControllerYawInput(Value);
    }
    

}



void ALegoCharacter::lookUp(float Value)
{
    if (Sling_Mode_Enable) {

        Sling_Camera->AddLocalOffset(FVector(0, 0, Value * -10));
    }
    else {
        AddControllerPitchInput(Value);
    }

}

void ALegoCharacter::zoom(float Value)
{
    if (Builder_Mode_Enable) {
        Builder_Camera->AddLocalOffset(FVector(0, 0, Value * 10));
    }
    else if (Sling_Mode_Enable) {
        Sling_Camera->AddLocalOffset(FVector(Value * 10, 0, 0));
    }


}



void ALegoCharacter::fire()
{
    Projectile_Item = Grabbable_Brick;
    if (Builder_Mode_Enable) {

        ABullet* bullet_ptr;
        FVector spawn_location = Builder_Camera->GetComponentLocation();
        FRotator spawn_rotation = Builder_Camera->GetForwardVector().Rotation();

        bullet_ptr = GetWorld()->SpawnActor<ABullet>(Bullet_Container, spawn_location, spawn_rotation);

        if (bullet_ptr != nullptr) {
            bullet_ptr->Strenght_ = 40;
            bullet_ptr->addFireImpulse(Builder_Camera->GetForwardVector().Rotation().Vector(), 30000);

        }
    }
  

}



void ALegoCharacter::changeCameraMode()
{
    Camera_Mode = (CameraMode)((int)Camera_Mode + 1);
    APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    switch (Camera_Mode)
    {
    case CameraMode::normal:
        Camera->SetActive(true);
        Builder_Camera->SetActive(false);
        Sling_Camera->SetActive(false);
        controller->bShowMouseCursor = false;

        Builder_Mode_Enable = false;
        Sling_Mode_Enable = false;

        buildEnd();
        break;

    case CameraMode::builder:
        Camera->SetActive(false);
        Builder_Camera->SetActive(true);
        Sling_Camera->SetActive(false);
        controller->bShowMouseCursor = false;

        Builder_Mode_Enable = true;
        Sling_Mode_Enable = false;

        buildStart();

        break;

    case CameraMode::sling:

        Camera->SetActive(false);
        Builder_Camera->SetActive(false);
        Sling_Camera->SetActive(true);
        controller->bShowMouseCursor = true;
        Builder_Mode_Enable = false;
        Sling_Mode_Enable = true;

        buildEnd();

        break;

    default:
        Camera_Mode = CameraMode::normal;

        Camera->SetActive(true);
        Builder_Camera->SetActive(false);
        Sling_Camera->SetActive(false);

        Builder_Mode_Enable = false;
        Sling_Mode_Enable = false;
        controller->bShowMouseCursor = false;

        buildEnd();
       
        break;
    }


}



void ALegoCharacter::aimEnd()
{
    Camera->SetActive(true);
    Builder_Camera->SetActive(false);

    Builder_Mode_Enable = false;

}












