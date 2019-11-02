// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"


UCLASS()
class DUNGEON_SIEGE_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	class UColliderMovementComponent* OurMovementComponent;

public:
	//Getter and Setter methods for static mesh and Sphere Component
	FORCEINLINE UStaticMeshComponent* GetMeshComponent() { return MeshComponent; }
	FORCEINLINE void SetMeshComponent(UStaticMeshComponent* Mesh) { MeshComponent = Mesh; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return SphereComponent; }
	FORCEINLINE void SetSphereComponent(USphereComponent* SphereMesh) { SphereComponent = SphereMesh; }


	//Getter and Setter methods for Camera and SpringArm
	FORCEINLINE UCameraComponent* GetCameraComponent() { return CameraComponent; }
	FORCEINLINE void SetCameraComponent(UCameraComponent* Camera) { CameraComponent = Camera; }
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() { return SpringArmComponent; }
	FORCEINLINE void SetSpringComponent(USpringArmComponent* SpringArm) { SpringArmComponent = SpringArm; }

	//virtual UPawnMovementComponent* GetMovementComponent() const override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void CameraPitch(float value);
	void CameraYaw(float value);
	FVector2D CameraInput;
};
