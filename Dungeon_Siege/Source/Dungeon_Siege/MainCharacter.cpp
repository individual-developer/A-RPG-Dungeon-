// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Pull toward the player if collision occur
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->TargetArmLength = 300.f; // Camera follow this distance
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
	MaxMagic = 1.f;
	Magic = 0.5f;
	Coins = 0;
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
	//PlayerInputComponent->BindAction("SwordSwing", IE_Pressed, this, &ACharacter);


}

void AMainCharacter::MoveForward(float value)
{
	if (Controller && value != 0.f)
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
	if (Controller && value != 0.f)
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

