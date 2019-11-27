// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainCharacter.h"

AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	LeftStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftStaticMesh"));
	LeftStaticMesh->SetupAttachment(GetRootComponent());

	RightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightStaticMesh"));
	RightStaticMesh->SetupAttachment(GetRootComponent());

	bWeaponParticle = false;

	WeaponState = EWeaponState::EWS_PickUp;
}

void AWeapon::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((WeaponState == EWeaponState::EWS_PickUp) && OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(nullptr);
		}
		
	}
}

void AWeapon::LeftEquip(AMainCharacter* character)
{
	LeftStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	LeftStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	LeftStaticMesh->SetSimulatePhysics(false);
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

void AWeapon::RightEquip(AMainCharacter* character)
{
	RightStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	RightStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	RightStaticMesh->SetSimulatePhysics(false);

	const USkeletalMeshSocket* RightHandSocket = character->GetMesh()->GetSocketByName("Sword_hold");
	if (RightHandSocket) {
		RightHandSocket->AttachActor(this, character->GetMesh());
		bRotate = false;
		character->SetEquipped(this);
		character->SetActiveOverlappingItem(nullptr);
	}

	if (OnEquippedSound)
		UGameplayStatics::PlaySound2D(this, OnEquippedSound);

	if (!bWeaponParticle)
		ParticleComponent->Deactivate();
}

 