// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "Enemy.h"
#include "AttackWeapon.h"
#include "DefenseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Pull toward the player if collision occur
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 450.f; // Camera follow this distance
	CameraArm->bUsePawnControlRotation = true; // Rotate is based on the controller

	//Set size for collision of Capsule
	GetCapsuleComponent()->SetCapsuleSize(48.f, 150.f);

	//creat FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	//Attach camera to the end of the arm, let arm to adjust to match the 
	//controller orientation
	FollowCamera->bUsePawnControlRotation = false;

	//Set up turn rate for input
	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	//Do not rotate when controller rotate
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // character move in the direction of input
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//...add this rotation rate
	GetCharacterMovement()->JumpZVelocity = 330.f;
	GetCharacterMovement()->AirControl = 0.2f;

	MaxHealth = 5.f;
	Health = 2.5f;
	MaxStamina = 1.f;
	Stamina = 0.5f;
	Coins = 0;
	RunningSpeed = 750.f;
	SprintSpeed = 950.f;
	bShiftKeyDown = false;
	bLMBDown = false;

	//Initialize Enum
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 0.12f;
	MinSprintStamina = 0.25f;

	bJumpKey = false;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

}

void AMainCharacter::ShowPickupLocations()
{
	for (int32 i = 0; i < PickupLocations.Num(); i++) {
		UKismetSystemLibrary::DrawDebugSphere(this,
			PickupLocations[i], 25.f, 8, FLinearColor::Green, 10.f, 0.5f);
	}
	
}

void AMainCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AMainCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMainCharacter::SetMovementStatus(EMovementStatus status)
{
	MovementStatus = status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;

	}
}

void AMainCharacter::DecrementHealth(float amout)
{
	if (Health - amout <= 0.f) {
		Health -= amout;
		Die();
	}
	else
		Health -= amout;
}

void AMainCharacter::IncrementCoinsCount(int amount)
{
	Coins += amount;
}

void AMainCharacter::Die()
{

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DeltaStamina = StaminaDrainRate * DeltaTime;
	switch (StaminaStatus)
	{
		case EStaminaStatus::ESS_Normal:
			Stamina_Normal(DeltaStamina);
			break;

		case EStaminaStatus::ESS_BelowMinimum:
			Stamina_BelowMinimum(DeltaStamina);
			break;

		case EStaminaStatus::ESS_Exhausted:
			Stamina_Exhausted(DeltaStamina);
			break;

		case EStaminaStatus::ESS_ExhaustedReplenishing:
			Stamina_ExhaustedReplenishing(DeltaStamina);
			break;

		default:
			;

	}
	if (bInterpToEnemy && CombatTarget) {
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainCharacter::LookUpRate);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::PressedJump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::ReleaseJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("MMB", IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction("MMB", IE_Released, this, &AMainCharacter::LMBUp);


	//PlayerInputComponent->BindAction("SwordSwing", IE_Pressed, this, &ACharacter);


}

void AMainCharacter::MoveForward(float value)
{
	if (Controller && value != 0.f&& !bAttacking)
	{
		//Find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, rotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}

void AMainCharacter::MoveRight(float value)
{
	if (Controller && value != 0.f&& !bAttacking)
	{
		//Find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, rotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, value);
	}
}

void AMainCharacter::TurnAtRate(float value)
{
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpRate(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::PressedJump()
{
	bPressedJump = true;
}

void AMainCharacter::ReleaseJump()
{
	bPressedJump = false;
}

void AMainCharacter::LMBDown()
{
	if (!bAttacking)
	{
		bLMBDown = true;
		if (ActiveOverlappingItem) {

			AAttackWeapon* AttackWeapon = Cast<AAttackWeapon>(ActiveOverlappingItem);
			ADefenseWeapon* DefenseWeapon = Cast<ADefenseWeapon>(ActiveOverlappingItem);

			if (AttackWeapon) {
				//FString name = weapon->GetName();
				//UE_LOG(LogTemp, Warning, TEXT("You pick up %s"), *name);
				//if (name == "Shield_BP") {
				AttackWeapon->Equip(this);
				SetActiveOverlappingItem(nullptr);
			}

			else if (DefenseWeapon) {
				DefenseWeapon->Equip(this);
				SetActiveOverlappingItem(nullptr);
			}

		}

		else if (RightEquippedWeapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("weapon is equipped"));
			Attack();
			//SetActiveOverlappingItem(nullptr);

		}
	}
}

void AMainCharacter::LMBUp()
{
	bLMBDown = false;

}

void AMainCharacter::SetEquipped(AItem* WeaponToSet)
{
	//if (EquippedWeapon)
		//EquippedWeapon->Destroy();
	if (AAttackWeapon* AttackWeapon = Cast<AAttackWeapon>(WeaponToSet))
		RightEquippedWeapon = WeaponToSet;
	else
		LeftEquippedWeapon = WeaponToSet;
	EquippedWeapon = WeaponToSet;
}


void AMainCharacter::Attack()
{
	if (!bAttacking)
	{
		bAttacking = true;
		SetInterpToEnemy(true);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			int32 section = FMath::RandRange(0, 1);
			switch (section) 
			{

			case 0:
			{
				AnimInstance->Montage_Play(CombatMontage, 1.f);
				AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
				break;
			}
			case 1:
			{
				AnimInstance->Montage_Play(CombatMontage, 0.9f);
				AnimInstance->Montage_JumpToSection(FName("Attack2"), CombatMontage);
				break;
			}
			default:
				;
			}

		}
	}

	

	
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown) {
		Attack();
	}

}

void AMainCharacter::PlaySwingSound()
{
	AAttackWeapon* attackWeapon = Cast<AAttackWeapon>(RightEquippedWeapon);
	if (attackWeapon) {
		if (attackWeapon->SwingSound) {
			UGameplayStatics::PlaySound2D(this, attackWeapon->SwingSound);
		}
	}
}

void AMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainCharacter::Stamina_Normal(float DeltaStamina)
{
	if (bShiftKeyDown)
	{
		if (Stamina - DeltaStamina <= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			Stamina -= DeltaStamina;
		}

		else
		{
			Stamina -= DeltaStamina;
		}

		SetMovementStatus(EMovementStatus::EMS_Sprinting);
	}

	//shift key up
	else
	{
		if (Stamina + DeltaStamina >= MaxStamina)
		{
			Stamina = MaxStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);

	}

}

void AMainCharacter::Stamina_BelowMinimum(float DeltaStamina)
{
	if (bShiftKeyDown)
	{
		if (Stamina - DeltaStamina <= 0.f)
		{
			Stamina = 0.f;
			SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		else
		{
			Stamina -= DeltaStamina;
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
	}
	else
	{

		if (Stamina + DeltaStamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
			Stamina += DeltaStamina;
		}
		else
		{
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
	}
}

void AMainCharacter::Stamina_Exhausted(float DeltaStamina)
{
	if (bShiftKeyDown) {
		Stamina = 0.f;
	}
	else
	{
		Stamina += DeltaStamina;
		SetStaminaStatus(EStaminaStatus::ESS_ExhaustedReplenishing);
	}
	SetMovementStatus(EMovementStatus::EMS_Normal);
}

void AMainCharacter::Stamina_ExhaustedReplenishing(float DeltaStamina)
{
	
	if (Stamina + DeltaStamina >= MinSprintStamina)
	{
		SetStaminaStatus(EStaminaStatus::ESS_Normal);
		Stamina += DeltaStamina;
	}
	else
	{
		Stamina += DeltaStamina;
	}
	SetMovementStatus(EMovementStatus::EMS_Normal);

}
