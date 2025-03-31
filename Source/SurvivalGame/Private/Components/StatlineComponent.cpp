#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	OwningCharMovementComp->MaxWalkSpeed = WalkSpeed;
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
	TickStamina((DeltaTime));
	TickHunger(DeltaTime);
	TickThirst(DeltaTime);
	
	if (!(Hunger.GetCurrent() <= 0.0) && !(Thirst.GetCurrent() <= 0.0)) // if neither hunger or thirst < 0 ; tick health regen
	{
		Health.TickStat((DeltaTime)); // health/stamina etc are of type FCoreStats struct || .TickStat is function within struct 
	}
}

void UStatlineComponent::TickStamina(const float& DeltaTime)
{	
	if (CurrentStaminaExh > 0.0) // TODO: can still sprint while in exhaustion (can sprint when no stamina)
	{
		CurrentStaminaExh -= DeltaTime;

		return;
	}
	
	if (bIsSprinting && IsValidSprinting())
	{
		Stamina.TickStat(0 - (DeltaTime * SprintCostMultiplier));

		if (Stamina.GetCurrent() <= 0.0)
		{
			SetSprinting(false);
			CurrentStaminaExh = StaminaExhDuration;
		}
		
		return ;
	}
	
	Stamina.TickStat(DeltaTime);
}

void UStatlineComponent::TickHunger(const float& DeltaTime)
{
	if (Hunger.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - (StarvationDamage * DeltaTime));
		return;
	}

	Hunger.TickStat(DeltaTime);
}

void UStatlineComponent::TickThirst(const float& DeltaTime)
{
	if (Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - (DehydrationDamage * DeltaTime));
		return;
	}

	Thirst.TickStat(DeltaTime);
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

	if (bIsSneaking && !bIsSprinting)
	{
		return;
	}
	
	bIsSneaking = false;

	OwningCharMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed; // if sprinting is true, set max walk to sprint, otherwise set max walk to walk
}

void UStatlineComponent::SetSneaking(const bool& IsSneaking)
{
	bIsSneaking = IsSneaking;

	if (bIsSprinting && !bIsSneaking)
	{
		return;
	}
	
	bIsSprinting = false;
	
	OwningCharMovementComp->MaxWalkSpeed = bIsSneaking ? SneakSpeed : WalkSpeed;
}


bool UStatlineComponent::IsValidSprinting()
{
	return OwningCharMovementComp->Velocity.Length() > WalkSpeed && !OwningCharMovementComp->IsFalling(); // are we moving faster than walk speed (aka running) but also not falling
}

bool UStatlineComponent::CanJump()
{
	return Stamina.GetCurrent() >= JumpCost && !OwningCharMovementComp->IsFalling(); // do we have enough stamina to pay jump cost
}

void UStatlineComponent::HasJumped()
{
	Stamina.Adjust(0 - JumpCost);
}
