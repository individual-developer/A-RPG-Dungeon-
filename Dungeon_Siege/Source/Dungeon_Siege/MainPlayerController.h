// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_SIEGE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	/**Reference to the UMG Asset*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<class UUserWidget> HUDOverlayAsset;

	/**Variable to hold widgets*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool EnemyHealthBarVisibility;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	

	
};
