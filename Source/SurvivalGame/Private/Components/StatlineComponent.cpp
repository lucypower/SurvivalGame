// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UStatlineComponent::UStatlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat((DeltaTime)); // health/stamina etc are of type FCoreStats struct || .TickStat is function within struct 
	TickStamina((DeltaTime)); // 
	Hunger.TickStat((DeltaTime));
	Thirst.TickStat((DeltaTime));
}

void UStatlineComponent::TickStamina(const float& DeltaTime)
{
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));

		return ;
	}
	Stamina.TickStat(DeltaTime);
}

bool UStatlineComponent::IsValidSprinting()
{
	return OwningCharMovementComp->Velocity.Length() > WalkSpeed && !OwningCharMovementComp->IsFalling(); // are we moving faster than walk speed (aka running) but also not falling
}

void UStatlineComponent::SetMovementCompRef(UCharacterMovementComponent* MovementCompRef)
{
	OwningCharMovementComp = MovementCompRef;
}

float UStatlineComponent::GetStatPercentile(const ECoreStats Stat) const
{
	switch (Stat)
	{
	case ECoreStats::CS_Health:
		return Health.Percentile();
		
	case ECoreStats::CS_Hunger:
		return Hunger.Percentile();

	case ECoreStats::CS_Stamina:
		return Stamina.Percentile();
		
	case ECoreStats::CS_Thirst:
		return Thirst.Percentile();
		
	default:
		// TODO: log invalid stat
			break;
	}

	return -1; // error value, should only hit this is the switch statement hits default case
}

bool UStatlineComponent::CanSprint() const
{
	return Stamina.GetCurrent() > 0.0; // gets whether the player has more than 0 stamina, if true, player will be able to sprint
}

void UStatlineComponent::SetSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;

	OwningCharMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed; // if sprinting is true, set max walk to sprint, otherwise set max walk to walk
}

bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost; // do we have enough stamina to pay jump cost
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
