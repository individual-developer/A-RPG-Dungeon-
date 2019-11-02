// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//check everything is still valid;
	if (!PawnOwner || !UpdatedComponent ||ShouldSkipUpdate(DeltaTime)) {
		return;
	}
	
	FVector DesiredMovementThisFame = ConsumeInputVector().GetClampedToMaxSize(1.f) * DeltaTime * 400.f;

	if (!DesiredMovementThisFame.IsNearlyZero()) {
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFame, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit()) {
			SlideAlongSurface(DesiredMovementThisFame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
