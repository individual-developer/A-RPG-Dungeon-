// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/StaticMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MainCharacter.h"
#include "Enemy.h"

//#include "AttackWeapon.h"

AAttackWeapon::AAttackWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	
	bWeaponParticle = false;

	WeaponState = EWeaponState_::EWS_PickUp;
	
	Damage = 25.f;
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

void AAttackWeapon::CombatBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy) {
			if (Enemy->HitParticle) {
				//const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("Sword_Socket");
				const UStaticMeshSocket* WeaponSocket = StaticMesh->GetSocketByName("Sword_Socket");
				if (WeaponSocket) {
					FTransform transform;
					bool SocketLocation = WeaponSocket->GetSocketTransform(transform, StaticMesh);
					FVector Location = transform.GetLocation();
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticle, Location, FRotator(0.f), false);
				}
				//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticle, GetActorLocation(), FRotator(0.f), false);
				
			}
			if (Enemy->HitSound) {
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound);
			}

			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
			}
		}
	}
}

void AAttackWeapon::CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AAttackWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AAttackWeapon::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAttackWeapon::Equip(AMainCharacter* character)
{
	if (character)
	{
		SetInstigator(character->GetController());
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
}

void AAttackWeapon::BeginPlay()
{
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AAttackWeapon::CombatBeginOverlap);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AAttackWeapon::CombatEndOverlap);
	
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

