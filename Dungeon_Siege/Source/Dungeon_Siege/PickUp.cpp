// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"
#include "Engine/World.h"
//#include "PickUp.h"


APickUp::APickUp()
{
	CoinCount = 1;
}

AMainCharacter* APickUp::GetCharacter() const
{
	return Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void APickUp::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto ControlledCharacter = GetCharacter();
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//UE_LOG(LogTemp, Warning, TEXT("APickUp::BeginOverlap with %s"), *ControlledCharacter->GetName());
	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->IncrementCoinsCount(CoinCount);
			Main->PickupLocations.Add(GetActorLocation());
			Destroy();
		}
	}

}

void APickUp::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto ControlledCharacter = GetCharacter();
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("APickUp::EndOverlap with %s"), *ControlledCharacter->GetName());

}
