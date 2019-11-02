// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"

// Sets default values
AFloater::AFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	InitialLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	bInitializeFloaterLocations = false;
}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();
	PlacedLocation = GetActorLocation();
	if(bInitializeFloaterLocations)
		SetActorLocation(InitialLocation);

}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

