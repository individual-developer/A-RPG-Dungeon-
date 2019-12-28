// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "engine/World.h"
#include "Critter.h"
#include "Enemy.h"
#include "AIController.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	if (ActorToSpawn_1 && ActorToSpawn_2 && ActorToSpawn_3 && ActorToSpawn_4) {
		SpawnArray.Add(ActorToSpawn_1);
		SpawnArray.Add(ActorToSpawn_2);
		SpawnArray.Add(ActorToSpawn_3);
		SpawnArray.Add(ActorToSpawn_4);
	}
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0) {
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
		return SpawnArray[Selection];
	}
	return nullptr;
}

void ASpawnVolume::SpawnActors_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn) {
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		if (World)
		{
			AActor* actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);
			if (AEnemy* enemy = Cast<AEnemy>(actor)) {
				enemy->SpawnDefaultController();
				AAIController* AI = Cast<AAIController>(enemy->GetController());
				if (AI) {
					enemy->AIController = AI;
				}
			}
			//ACritter* CritterSpawned = Cast<ACritter>(World->SpawnActor<APawn>(ToSpawn, Location, FRotator(0.f), SpawnParams));
			//ACritter* CritterSpawned = reinterpret_cast<ACritter*>(World->SpawnActor<APawn>(ToSpawn, Location, FRotator(0.f), SpawnParams));
		}
	}
}

