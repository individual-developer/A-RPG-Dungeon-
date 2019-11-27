// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_PickUp		UMETA(DisplayName = "PickUp"),
	EWS_Equipped	UMETA(DisplayName = "Equipped"),
	EWS_MAX			UMETA(DisplayName = "DefaultMAX")
};
	
/**
 * 
 */
UCLASS()
class DUNGEON_SIEGE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Particles")
	bool bWeaponParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Sound")
	class USoundCue* OnEquippedSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category= "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LeftStaticMesh")
	class UStaticMeshComponent* LeftStaticMesh;
	//class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "RightStaticMesh")
	class UStaticMeshComponent* RightStaticMesh;

	
	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	
	void LeftEquip(class AMainCharacter* character);
	void RightEquip(class AMainCharacter* character);
	FORCEINLINE void SetWeaponState(EWeaponState State) { WeaponState = State; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

};
