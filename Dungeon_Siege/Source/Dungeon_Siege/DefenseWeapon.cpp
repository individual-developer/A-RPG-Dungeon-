// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainCharacter.h"


ADefenseWeapon::ADefenseWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());


	bWeaponParticle = false;

	WeaponState = EWeaponState__::EWS_PickUp;
}

void ADefenseWeapon::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((WeaponState == EWeaponState__::EWS_PickUp) && OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void ADefenseWeapon::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(nullptr);
		}

	}
}

void ADefenseWeapon::Equip(AMainCharacter* character)
{
	
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetSimulatePhysics(false);
	//SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//SkeletalMesh->SetSimulatePhysics(false);

	const USkeletalMeshSocket* LeftHandSocket = character->GetMesh()->GetSocketByName("Shield_hold");
	if (LeftHandSocket) {
		LeftHandSocket->AttachActor(this, character->GetMesh());
		bRotate = false;
		character->SetEquipped(this);
		character->SetActiveOverlappingItem(nullptr);
	}

	if (OnEquippedSound)
		UGameplayStatics::PlaySound2D(this, OnEquippedSound);

	if (!bWeaponParticle)
		ParticleComponent->Deactivate();
}
