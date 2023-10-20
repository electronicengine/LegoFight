// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstraitBrick.h"

AConstraitBrick::AConstraitBrick() {

	Constrait = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constrait"));
	Additional_Brick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Additional Mesh"));

	Constrait->SetConstrainedComponents(Brick, NAME_None, Additional_Brick, NAME_None);

}