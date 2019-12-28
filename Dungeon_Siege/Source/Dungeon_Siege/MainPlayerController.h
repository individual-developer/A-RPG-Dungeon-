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

	UFUNCTION(BlueprintCallable)
	void RemoveHUD();

	UFUNCTION(BlueprintCallable)
	void DisplayHUD();

	UFUNCTION(BlueprintCallable)
	void RemoveLoading();

	UFUNCTION(BlueprintCallable)
	void DisplayLoading();

	UFUNCTION(BlueprintCallable)
	void RemoveLevelTransition();

	UFUNCTION(BlueprintCallable)
	void DisplayLevelTransition();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool EnemyHealthBarVisibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* PauseMenu;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WLevelTransition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* LevelTransition;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> WLoading;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* Loading;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool PauseMenuVisibility;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=HUD)
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = HUD)
	void RemovePauseMenu();

	FVector EnemyLocation;
	void TogglePauseMenu();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	

	
};
