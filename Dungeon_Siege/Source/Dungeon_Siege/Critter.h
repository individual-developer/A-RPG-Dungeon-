// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include <Engine.h>
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"


UCLASS()
class DUNGEON_SIEGE_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

private:
	void MoveForward(float value);
	void MoveRight(float value);
	FVector CurrentVelocity;
	

};
