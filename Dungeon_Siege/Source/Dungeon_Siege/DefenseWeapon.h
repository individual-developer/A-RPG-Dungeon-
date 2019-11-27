// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "DefenseWeapon.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWeaponState__ : uint8
{
	EWS_PickUp		UMETA(DisplayName = "PickUp"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
};

/**
 *  
 */
UCLASS()
class DUNGEON_SIEGE_API ADefenseWeapon : public AItem
{
	GENERATED_BODY()

public:
	ADefenseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particles")
	bool bWeaponParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState__ WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* OnEquippedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StaticMesh")
	class UStaticMeshComponent* StaticMesh;


	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	void Equip(class AMainCharacter* character);
	FORCEINLINE void SetWeaponState(EWeaponState__ State) { WeaponState = State; }
	FORCEINLINE EWeaponState__ GetWeaponState() { return WeaponState; }

};
