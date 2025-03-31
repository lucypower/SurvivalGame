#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

UENUM(BlueprintType)
enum class ECoreStats : uint8
{
	CS_Health UMETA(DisplayName = "Health"),
	CS_Stamina UMETA(DisplayName = "Stamina"),
	CS_Hunger UMETA(DisplayName = "Hunger"),
	CS_Thirst UMETA(DisplayName = "Thirst")
};

USTRUCT(BlueprintType) // can access variables in the editor
struct FCoreStats
{
	GENERATED_USTRUCT_BODY()

private :

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

public :

	FCoreStats() // default constructor
	{
	
	};

	FCoreStats(const float& current, const float& max, const float& tick) // override constructor
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
	};
	
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	void Adjust(const float& Amount)
	{
		Current = FMath::Clamp(Current + Amount, 0, Max);
	}

	float Percentile() const // for UI display I believe
	{
		return Current / Max;
	}

	void AdjustTick(const float& NewTick)
	{
		PerSecondTick = NewTick;
	}

	float GetCurrent() const
	{
		return Current;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private :

	class UCharacterMovementComponent* OwningCharMovementComp;

#pragma region Stats
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStats Health; // default values of FCoreStat values

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStats Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStats Hunger = FCoreStats(100, 100, -0.125); // override default values of FCoreStat values

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStats Thirst = FCoreStats(100, 100, -0.25);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintCostMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) // EditDefaultsOnly means I can edit the values in the class defaults panel in a blueprint
	float WalkSpeed = 125;																 // but the values cannot be edited at runtime

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 450;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 75;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float JumpCost = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) // want to edit it per bp but not at runtime
	float StaminaExhDuration = 5;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) // don't want to edit this but do want to see it
	float CurrentStaminaExh = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StarvationDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float DehydrationDamage = 1;
	
#pragma endregion

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	
	void TickStats(const float& DeltaTime);

	void TickStamina(const float& DeltaTime);

	void TickHunger(const float& DeltaTime);

	void TickThirst(const float& DeltaTime);
	
	bool IsValidSprinting();
	
protected:
	
	virtual void BeginPlay() override;

public:
	
	UStatlineComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMovementCompRef(UCharacterMovementComponent* MovementCompRef);
	
	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStats Stat) const;
	
	UFUNCTION(BlueprintCallable) // mostly for AI BT's 
	bool CanSprint()const;

	UFUNCTION(BlueprintCallable) // tell statline comp that it is sprinting or not
	void SetSprinting(const bool& IsSprinting);

	UFUNCTION(BlueprintCallable)
	void SetSneaking(const bool& IsSneaking);

	UFUNCTION(BlueprintCallable) // can we jump? useful for AI too
	bool CanJump();

	UFUNCTION(BlueprintCallable) // have we jumped? useful for AI too
	void HasJumped();
};

