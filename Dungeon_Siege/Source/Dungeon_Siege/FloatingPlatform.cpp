// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FloatPlatform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloatPlatForm"));
	RootComponent = FloatPlatform;
	StartPoint = FVector(0);
	EndPoint = FVector(0);
	InterpolateSpeed = 4.0f;
	//Distance = (EndPoint - StartPoint).Size();
	InterpTime = 1.f;
	//bInterp = false;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartPoint = GetActorLocation();
	EndPoint += StartPoint;
	bInterp = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterpolation, InterpTime);
	Distance = (EndPoint - StartPoint).Size();
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	if (bInterp) {
		FVector CurrentLocation = GetActorLocation();
		FVector Interpolation = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpolateSpeed);
		SetActorLocation(Interpolation);
		//FloatPlatform->SetWorldLocation(Interpolation);
		float DistanceTravel = (GetActorLocation() - StartPoint).Size();
		if (Distance - DistanceTravel <= 1.f) {
			ToggleInterpolation();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterpolation, InterpTime);
			SwappingVec(StartPoint, EndPoint);
		}
	}
	
	//SetActorLocation(Interpolation);
}

void AFloatingPlatform::ToggleInterpolation()
{
	bInterp = !bInterp;
}

void AFloatingPlatform::SwappingVec(FVector& Start, FVector& End)
{
	FVector Temp = Start;
	Start = End;
	End = Temp;
}

