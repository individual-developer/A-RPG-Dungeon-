// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "MainPlayerController.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ArgoSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("CombatSocket"));
	
	bOverlappingCombatSphere = false;
	EnemyMovementStatus = EEnemyMovementStatus::Idle;
	Health = 75.f;
	MAXHealth = 100.f;
	Damage = 0.5f;
	AttackMinTime = 0.5f;
	AttackMaxTime = 2.0f;
	DelayTime = 3.f;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereBeginOverlap);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereEndOverlap);
	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereBeginOverlap);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereEndOverlap);

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatBeginOverlap);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatEndOverlap);

	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
		if (Main) {
			MoveToTarget(Main);
		}
	}
}

void AEnemy::AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* main = Cast<AMainCharacter>(OtherActor);
		if (main) {
			main->SetCombatTarget_(nullptr);
			main->SetHasCombatTarget(false);
			SetEnemyMovementStatus(EEnemyMovementStatus::Idle);
			//MoveToTarget(main);
			if (AIController) {
				AIController->StopMovement();
			}
			if (main->MainPlayerController) {
				main->MainPlayerController->RemoveEnemyHealthBar();
			}
		}
	}
}

void AEnemy::CombatSphereBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		AMainCharacter* main = Cast<AMainCharacter>(OtherActor);
		if (main) {
			main->SetCombatTarget(this);
			main->SetCombatTarget_(this);
			//main->bHasCombatTarget = true;
			main->SetHasCombatTarget(true);

			if (main->MainPlayerController) {
				main->MainPlayerController->DisplayEnemyHealthBar();
			}
			CombatTarget = main;
			bOverlappingCombatSphere = true;
			//SetEnemyMovementStatus(EEnemyMovementStatus::Attacking);
			Attack();
		}
	}
}

void AEnemy::CombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* main = Cast<AMainCharacter>(OtherActor);
		if (main) {
			if (main->CombatTarget == this) {
				main->SetCombatTarget(nullptr);
				//main->bHasCombatTarget = false;
				//main->SetHasCombatTarget(false);

			}

			bOverlappingCombatSphere = false;
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			//MoveToTarget(main);
			if (EnemyMovementStatus != EEnemyMovementStatus::Attacking) {
				MoveToTarget(main);
				CombatTarget = nullptr;
			}
			GetWorldTimerManager().ClearTimer(AttackTimer);
		}
		
	}
}

void AEnemy::MoveToTarget(AMainCharacter* Main)
{
	if (Alive())
	{
		SetEnemyMovementStatus(EEnemyMovementStatus::MoveToTarget);
		if (AIController)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Move to Target"));
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Main);
			MoveRequest.SetAcceptanceRadius(15.0f);
			FNavPathSharedPtr NavPath;
			AIController->MoveTo(MoveRequest, &NavPath);
			//auto PathPoints = NavPath->GetPathPoints();
			//for (const auto& point : PathPoints) {
			//	UKismetSystemLibrary::DrawDebugSphere(this,
			//		point, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
			//}

		}
	}
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - Damage <= 0.f) {
		Health -= Damage;
		Die();
	}
	else {
		Health -= Damage;
	}

	return Damage;
}

void AEnemy::CombatBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Alive())
	{
		if (OtherActor) {
			AMainCharacter* Main = Cast<AMainCharacter>(OtherActor);
			if (Main) {
				if (Main->HitParticle) {
					//const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("Sword_Socket");
					const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
					if (TipSocket) {
						FVector TipSocketLocation = TipSocket->GetSocketLocation(GetMesh());
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticle, TipSocketLocation, FRotator(0.f), false);
					}

				}
				if (Main->HitSound) {
					UGameplayStatics::PlaySound2D(this, Main->HitSound);
				}
				if (DamageTypeClass) {
					UGameplayStatics::ApplyDamage(Main, Damage, AIController, this, DamageTypeClass);
				}

			}
		}
	}
}


void AEnemy::CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AEnemy::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (SwingSound) {
		UGameplayStatics::PlaySound2D(this, SwingSound);
	}
}

void AEnemy::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		//Attack();
	}
}

void AEnemy::Attack()
{
	if (Alive())
	{
		if (AIController) {
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::Attacking);
		}

		if (!bAttacking) {
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance && CombatMontage) {
				AnimInstance->Montage_Play(CombatMontage, 1.3f);
				AnimInstance->Montage_JumpToSection(FName("Attack1"), CombatMontage);
			}

		}
	}

}

void AEnemy::DeathEnd()
{
	//UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	//AnimInstance->Montage_Play(CombatMontage, 0.f);
	
	GetMesh()->bPauseAnims = true;
	GetWorldTimerManager().SetTimer(DeathDelayTimer, this, &AEnemy::Disappear, DelayTime);
	//GetMesh()->bNoSkeletonUpdate = true;
}

bool AEnemy::Alive()
{
	return GetEnemyMovementStatus() != EEnemyMovementStatus::Dead;
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.f);
		AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::Dead);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bAttacking = false;
}

void AEnemy::Disappear()
{
	Destroy();
}

