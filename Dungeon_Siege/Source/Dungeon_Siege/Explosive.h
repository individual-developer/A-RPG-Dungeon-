// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEON_SIEGE_API AExplosive : public AItem
{
	GENERATED_BODY()
public:
	AExplosive();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Damage)
	float Damage;
	AMainCharacter* GetCharacter()const;
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<class UDamageType> DamageTypeClass;
};
