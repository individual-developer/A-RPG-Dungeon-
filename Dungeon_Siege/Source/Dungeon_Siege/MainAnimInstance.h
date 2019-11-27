// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
//#include"MainCharacter.h"
#include "MainAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class DUNGEON_SIEGE_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation()override;

	UFUNCTION(BlueprintCallable, category=AnimationProperties)
	void UpdateAnimationProperties();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Movement)
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	bool IsJumpKeyPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AMainCharacter* Main;

	AMainCharacter* GetMainCharacter()const;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsSwing;




};
