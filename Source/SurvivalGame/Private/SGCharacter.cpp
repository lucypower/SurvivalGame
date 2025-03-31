// Fill out your copyright notice in the Description page of Project Settings.


#include "SGCharacter.h"
#include "Components/StatlineComponent.h"

// Sets default values
ASGCharacter::ASGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementCompRef(GetCharacterMovement()); // sets character movement component reference for the statline component
}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ASGCharacter::CanJump() const
{
	return Statline->CanJump();
}

void ASGCharacter::HasJumped()
{
	Statline->HasJumped();

	ACharacter::Jump();
}

bool ASGCharacter::CanSprint() const
{
	return Statline->CanSprint();
}

void ASGCharacter::SetSprinting(const bool& IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}

// Called every frame
void ASGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

