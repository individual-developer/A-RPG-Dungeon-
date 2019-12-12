// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "Blueprint/UserWidget.h"
//#include "MainCharacter.h"

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


}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyHealthBar) {
		FVector2D PositionViewPort;
		ProjectWorldLocationToScreen(EnemyLocation, PositionViewPort);
		PositionViewPort.Y -= 80;
		PositionViewPort.X -= 20;

		FVector2D SizeInViewPort(200.f, 15.f);
		EnemyHealthBar->SetPositionInViewport(PositionViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
	
}
