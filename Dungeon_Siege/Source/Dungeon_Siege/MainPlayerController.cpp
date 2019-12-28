// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
//#include "MainCharacter.h"

void AMainPlayerController::RemoveHUD()
{
	if (HUDOverlay) {
		HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayHUD()
{
	if (HUDOverlay) {
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveLoading()
{
	if (Loading) {
		Loading->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayLoading()
{
	if (Loading) {
		Loading->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveLevelTransition()
{
	if (LevelTransition)
	{
		LevelTransition->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayLevelTransition()
{
	if (LevelTransition)
	{
		LevelTransition->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::DisplayEnemyHealthBar()
{
	if (EnemyHealthBar)
	{
		EnemyHealthBarVisibility = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar) {
		EnemyHealthBarVisibility = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
	if (PauseMenu)
	{
		PauseMenuVisibility = true;
		PauseMenu->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameAndUI;
		//FInputModeUIOnly InputModeUI;
		SetInputMode(InputModeGameAndUI);
		
		bShowMouseCursor = true;
		
		
	}
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
	if (PauseMenu)
	{
		PauseMenuVisibility = false;
		PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}

void AMainPlayerController::TogglePauseMenu()
{
	if (PauseMenuVisibility) {
		RemovePauseMenu();
	}

	else
		DisplayPauseMenu();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	
	//HUDOverlay->SetAnchorsInViewport();
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar) {
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar) {
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
		}
		FVector2D Alignment(0.f, 0.f);
		EnemyHealthBar->SetAlignmentInViewport(Alignment);
	}

	if (WPauseMenu) {
		PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
		if (PauseMenu) {
			PauseMenu->AddToViewport();
			PauseMenu->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}

	if (WLevelTransition)
	{
		LevelTransition = CreateWidget<UUserWidget>(this, WLevelTransition);
		if (LevelTransition) {
			LevelTransition->AddToViewport();
			LevelTransition->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (WLoading)
	{
		Loading = CreateWidget<UUserWidget>(this, WLoading);
		if (Loading) {
			Loading->AddToViewport();
			Loading->SetVisibility(ESlateVisibility::Hidden);
		}
	}


}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyHealthBar) {
		FVector2D PositionViewPort;
		ProjectWorldLocationToScreen(EnemyLocation, PositionViewPort);
		PositionViewPort.Y -= 70;
		PositionViewPort.X -= 30;

		FVector2D SizeInViewPort(200.f, 15.f);
		EnemyHealthBar->SetPositionInViewport(PositionViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
	
}
