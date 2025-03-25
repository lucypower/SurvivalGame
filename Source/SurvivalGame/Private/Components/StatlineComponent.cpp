// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatlineComponent.h"

// Sets default values for this component's properties
UStatlineComponent::UStatlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
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
	Health.TickStat((DeltaTime)); // health/stamina etc are of type FCoreStats struct
	Stamina.TickStat((DeltaTime)); // .TickStat is function within struct 
	Hunger.TickStat((DeltaTime));
	Thirst.TickStat((DeltaTime));
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

