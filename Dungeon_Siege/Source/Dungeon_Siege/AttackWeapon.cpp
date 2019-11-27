// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainCharacter.h"

//#include "AttackWeapon.h"

AAttackWeapon::AAttackWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());


	bWeaponParticle = false;

	WeaponState = EWeaponState_::EWS_PickUp;
	
}

void AAttackWeapon::BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlapComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((WeaponState == EWeaponState_::EWS_PickUp) && OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(this);
		}
	}
}

void AAttackWeapon::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			Main->SetActiveOverlappingItem(nullptr);
		}

	}
}

void AAttackWeapon::Equip(AMainCharacter* character)
{
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	StaticMesh->SetSimulatePhysics(false);
	//SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	//SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//SkeletalMesh->SetSimulatePhysics(false);


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

