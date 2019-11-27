// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus :uint8
{
	EMS_Normal UMETA(DisplayName="Normal"),
	EMS_Sprinting UMETA(DisplayName="Sprinting"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal UMETA(DisplayName="Normal"),
	ESS_BelowMinimum UMETA(DisplayName="BelowMinimum"),
	ESS_Exhausted UMETA(DisplayName="Exhasuted"),
	ESS_ExhaustedReplenishing UMETA(DisplayName="ExhasutedReplenishing"),
	ESS_MAX UMETA(DisplayName="DefaultMAX")
};

UCLASS()
class DUNGEON_SIEGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	TArray<FVector> PickupLocations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Items)
	class AItem* EquippedWeapon; // change code


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items)
	class AItem* ActiveOverlappingItem;

	UFUNCTION(BlueprintCallable)
	void ShowPickupLocations();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Enums)
	EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus status) { StaminaStatus = status; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float StaminaDrainRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MinSprintStamina;

	/**Set movement status and running speed*/
	void SetMovementStatus(EMovementStatus status);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Running)
	float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sprinting)
	float SprintSpeed;

	bool bShiftKeyDown;
	/**Press shift key down to enable sprinting*/
	void ShiftKeyDown();
	/**Release to stop sprinting*/
	void ShiftKeyUp();

	void Stamina_Normal(float DeltaStamina);
	void Stamina_BelowMinimum(float DeltaStamina);
	void Stamina_Exhausted(float DeltaStamina);
	void Stamina_ExhaustedReplenishing(float DeltaStamina);

	/**CameraArm positions the Camera behind the player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	/**Follow camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//Base turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/************************************************************************/
	/* Player Stats                                                                     */
	/************************************************************************/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	void DecrementHealth(float amout);
	void IncrementCoinsCount(int amount);
	void Die();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Call for the move forward and backward input
	void MoveForward(float value);

	//Call for the move side by side input
	void MoveRight(float value);
	
	// Called input to turn at a given rate
	void TurnAtRate(float value);

	// Called input to look up/down at a given rate, 1.0 represents the 100% rate
	void LookUpRate(float value);

	bool bJumpKey;
	void PressedJump();
	void ReleaseJump();
	
	bool bLMBDown;

	void LMBDown();

	void LMBUp();

	FORCEINLINE USpringArmComponent* GetCameraArm() const { return CameraArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
	void SetEquipped(AItem* WeaponToSet);
	//FORCEINLINE AWeapon* Equipped(){ return LeftEquippedWeapon; }
	FORCEINLINE AItem* GetEquipped() { return EquippedWeapon; }
	//FORCEINLINE void SetLeftActiveOverlappingItem(AItem* Item) { LeftActiveOverlappingItem = Item; }
	FORCEINLINE void SetActiveOverlappingItem(AItem* Item) { ActiveOverlappingItem = Item; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Anims)
	bool bAttacking;

	void Attack();

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Anims)
	class UAnimMontage* CombatMontage;
};
