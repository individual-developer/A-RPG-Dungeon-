// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

UCLASS()
class DUNGEON_SIEGE_API AFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloater();
	UPROPERTY(VisibleAnywhere, Category="ActorMeshComponents")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	FVector InitialLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Floater Variables")
	//Location of the actor when dragged in from the editor
	FVector PlacedLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Floater Variables")
	bool bInitializeFloaterLocations;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
