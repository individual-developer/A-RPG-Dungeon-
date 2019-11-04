// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ColliderMovementComponent.h"


// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(40.f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	if (MeshComponentAsset.Succeeded()) {
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetSpringArmComponent(), USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("MovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	CameraInput = FVector2D(0.f, 0.f);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator NewRotarion = GetActorRotation();
	NewRotarion.Yaw += CameraInput.X;
	FRotator newSpringArmRotation = SpringArmComponent->GetComponentRotation();
	newSpringArmRotation.Pitch = FMath::Clamp(newSpringArmRotation.Pitch += CameraInput.Y, -80.f, 30.f);
	//NewRotarion.Pitch += CameraInput.Y;
	SpringArmComponent->SetWorldRotation(newSpringArmRotation);
	SetActorRotation(NewRotarion);
	

}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::CameraYaw);
}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollider::MoveForward(float value)
{
	FVector Forward = GetActorForwardVector();
	//AddMovementInput(value * Forward);
	if(GetMovementComponent())
		OurMovementComponent->AddInputVector(value * Forward);
}

void ACollider::CameraPitch(float value) {
	CameraInput.Y = value;
}

void ACollider::CameraYaw(float value) {
	CameraInput.X = value;
}

void ACollider::MoveRight(float value)
{
	FVector Right = GetActorRightVector();
	//AddMovementInput(value * Right);
	
	if (GetMovementComponent())
	{
		OurMovementComponent->AddInputVector(value * Right);
	}

	
}

