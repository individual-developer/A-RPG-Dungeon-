// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Item.h"
#include "PickUp.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_SIEGE_API APickUp : public AItem
{
	GENERATED_BODY()

public:
	
	APickUp();
	UFUNCTION(BlueprintImplementableEvent, Category=Pickup)
	void OnPickUpBP(class AMainCharacter* main);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Coins)
	//int32 CoinCount;
	AMainCharacter* GetCharacter()const;
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	
};
