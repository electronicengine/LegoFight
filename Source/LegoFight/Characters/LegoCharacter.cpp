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
    Interaction_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnInteractBegin);
    Interaction_Component->OnComponentEndOverlap.AddDynamic(this, &ALegoCharacter::OnInteractEnd);

    Ghost_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ghost"));

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
}


// Called when the game starts or when spawned
void ALegoCharacter::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("ALegoCharacter::BeginPlay()"));

    Game_Instance = Cast<ULegoFightGameInstance>(GetWorld()->GetGameInstance());
    Game_Instance->loadCharacterPanel();
//    Skeletal_Mesh->AddRadialImpulse(GetActorLocation(), 100, 1000, RIF_Constant, true);
    Ghost_Component->OnComponentBeginOverlap.AddDynamic(this, &ALegoCharacter::OnGhostOverLap);


}

// Called every frame
void ALegoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (Aiming_ == true)
    {
        FHitResult OutHit;
        FVector Start = Aim_Camera->GetComponentLocation();
        FVector forward_vector = Aim_Camera->GetComponentRotation().Vector();
        FVector End = Start + (forward_vector * 3000.0f);
        FCollisionQueryParams CollisionParams;

        GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);
        Aim_Impact_Point = OutHit.ImpactPoint;

        lookForBuildingSpace(OutHit.Actor.Get(), OutHit);
    }

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
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ALegoCharacter::aimStart);
    //PlayerInputComponent->BindAction("Aim", IE_Released, this, &ALegoCharacter::aimEnd);

    PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &IInteractInterface::openInventoryWidget);
    PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &IInteractInterface::equip);
    PlayerInputComponent->BindAction("OffSetItem", IE_Pressed, this, &IBuilderInterface::giveOffsetLocation);
    PlayerInputComponent->BindAction("TurnObject", IE_Pressed, this, &IBuilderInterface::giveOffsetRotation);

    PlayerInputComponent->BindAction("Plug", IE_Pressed, this, &IBuilderInterface::plugObject);
    PlayerInputComponent->BindAction("Buy", IE_Pressed, this, &IInteractInterface::buyBrick);
    PlayerInputComponent->BindAction("Save", IE_Pressed, this, &IInteractInterface::saveAndLoad);
    PlayerInputComponent->BindAction("Load", IE_Pressed, this, &ALegoCharacter::saveAndLoad);

}



void ALegoCharacter::OnInteractBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{

    if (Aiming_ == false && Cast<IPlugInterface>(OtherActor) && !Cast<ALegoCharacter>(OtherActor)){
        interactNearby(OtherActor);
        Cast<UCharacterWidget>(Game_Instance->Char_Panel)->setInteractButtonVisibilty(ESlateVisibility::Visible);
    }

}

void ALegoCharacter::OnInteractEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
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
    AddControllerYawInput(Value );

}



void ALegoCharacter::lookUp(float Value)
{
    AddControllerPitchInput(Value);

}

void ALegoCharacter::zoom(float Value)
{
    if (Aiming_) {
        Aim_Camera->AddLocalOffset(FVector(0, 0, Value * 10));
    }


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
        buildEnd();
    }
    else {
        Camera->SetActive(false);
        Aim_Camera->SetActive(true);
        Aiming_ = true;
        buildStart();

    }

}



void ALegoCharacter::aimEnd()
{
    Camera->SetActive(true);
    Aim_Camera->SetActive(false);

    Aiming_ = false;

}












