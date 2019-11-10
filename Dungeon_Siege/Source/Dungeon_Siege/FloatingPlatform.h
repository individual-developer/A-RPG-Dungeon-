// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class DUNGEON_SIEGE_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlatForm")
	UStaticMeshComponent* FloatPlatform;

	UPROPERTY(EditAnywhere)
	FVector StartPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
	FVector EndPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlatForm")
	float InterpolateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlatForm")
	float InterpTime;
	
	FTimerHandle InterpTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlatForm")
	bool bInterp;

	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ToggleInterpolation();
	void SwappingVec(FVector& Start, FVector& End);

};
