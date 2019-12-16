// Fill out your copyright notice in the Description page of Project Settings.

#include "PickUp.h"
#include "Engine/World.h"
//#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MainCharacter.h"


APickUp::APickUp()
{
	//CoinCount = 1;
}

AMainCharacter* APickUp::GetCharacter() const
{
	return Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void APickUp::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	//For Debug
	//auto ControlledCharacter = GetCharacter();
	//UE_LOG(LogTemp, Warning, TEXT("APickUp::BeginOverlap with %s"), *ControlledCharacter->GetName());

	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {

			OnPickUpBP(Main);
			if (OverlapParticle) {
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle,GetActorLocation(), FRotator(0.f), true);
			}
			if (OverlapSound)
			{
				UGameplayStatics::PlaySound2D(this, OverlapSound);
			}

			//Main->IncrementCoinsCount(CoinCount);
			//Main->PickupLocations.Add(GetActorLocation()); //Debug Sphere
			Destroy();
		}
	}

}

void APickUp::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//For debug
	//auto ControlledCharacter = GetCharacter();
	//UE_LOG(LogTemp, Warning, TEXT("APickUp::EndOverlap with %s"), *ControlledCharacter->GetName());

}
