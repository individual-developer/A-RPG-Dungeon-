// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "COmponents/SphereComponent.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainCharacter.h"


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
	bOverlappingCombatSphere = false;


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
	if (OtherActor)
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
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			//MoveToTarget(main);
			if (AIController) {
				AIController->StopMovement();
			}
		}
	}
}

void AEnemy::CombatSphereBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* main = Cast<AMainCharacter>(OtherActor);
		if (main) {
			CombatTarget = main;
			bOverlappingCombatSphere = true;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
	}
}

void AEnemy::CombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* main = Cast<AMainCharacter>(OtherActor);
		if (main) {
			bOverlappingCombatSphere = false;
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			//MoveToTarget(main);
			if (EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking) {
				MoveToTarget(main);
				CombatTarget = NULL;
			}
		}
	}
}

void AEnemy::MoveToTarget(AMainCharacter* Main)
{
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AIController)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Move to Target"));
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Main);
		MoveRequest.SetAcceptanceRadius(15.0f);
		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveRequest, &NavPath);
		auto PathPoints = NavPath->GetPathPoints();
		for (const auto& point : PathPoints) {
			UKismetSystemLibrary::DrawDebugSphere(this,
				point, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
		}
		


	}
}

