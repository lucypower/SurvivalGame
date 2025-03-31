// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGCharacter.h"
#include "SGPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS(config=Game)
class SURVIVALGAME_API ASGPlayerCharacter : public ASGCharacter
{
	GENERATED_BODY()

private:

	// Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	#pragma region Input

	// Mapping Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	// Jump Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// Move Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// Look Input Action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SneakAction;

#pragma endregion Input
	
protected:

	// Called for movement input
	void Move(const FInputActionValue& Value);

	// Called for look input
	void Look(const FInputActionValue& Value);

	void PlayerJump();
	
	void SprintOn();
	void SprintOff();

	void SneakOn();
	void SneakOff();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Add mapping context
	virtual void BeginPlay();

public:

	ASGPlayerCharacter();
	
	// Return CameraBoom subobject
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Return FollowCamera subobject
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
