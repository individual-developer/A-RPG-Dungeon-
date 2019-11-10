// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"



// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/**Construct the TriggerBox*/
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	/**Set TriggerBox as the parent*/
	RootComponent = TriggerBox;
	
	/**Setup Collision*/
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(62.f, 62.f, 31.f));

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());
	Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());

	bCharacterOnSwitch = false;
	SwitchTime = 2.f;
}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::BeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::EndOverlap);
	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (TriggerBox->IsOverlappingComponent()) {
		float dz = 50.f * DeltaTime;
		float x = Door->GetComponentLocation().X;
		float y = Door->GetComponentLocation().Y;
		float z = Door->GetComponentLocation().Z;
		Door->SetWorldLocation(FVector(x, y, z + dz));
	}

	else
	{
		if(Door->GetComponentLocation().Z > InitialDoorLocation.Z)
		{
			float dz = 50.f * DeltaTime;
			float x = Door->GetComponentLocation().X;
			float y = Door->GetComponentLocation().Y;
			float z = Door->GetComponentLocation().Z;
			Door->SetWorldLocation(FVector(x, y, z - dz));
		}
	}*/

}

void AFloorSwitch::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin!"));
	if(!bCharacterOnSwitch)
		bCharacterOnSwitch = true;
	RaiseDoor();
	LowerSwitch();
}

void AFloorSwitch::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap End!"));
	if(bCharacterOnSwitch)
		bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwitchTime);
	//LowerDoor();
	//RaiseSwitch();
}

void AFloorSwitch::UpdateDoorLocation(float z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += z;
	Door->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchLocation(float z)
{
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += z;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterOnSwitch) {
		LowerDoor();
		RaiseSwitch();
	}
}

