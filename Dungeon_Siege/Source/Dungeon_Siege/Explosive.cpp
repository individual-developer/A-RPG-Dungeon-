// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/World.h"
#include "Explosive.h"

AExplosive::AExplosive()
{
	Damage = 0.5f;
}

AMainCharacter* AExplosive::GetCharacter() const
{
	return Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AExplosive::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto ControlledCharacter = GetCharacter();
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("AExplosive::BeginOverlap with %s"), *ControlledCharacter->GetName()) 
	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->DecrementHealth(Damage);
		}
	}
}

void AExplosive::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto ControlledCharacter = GetCharacter();
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("AExplosive::EndOverlap with %s"), *ControlledCharacter->GetName());
}
