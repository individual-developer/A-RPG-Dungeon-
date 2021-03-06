// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharacter.h"
//#include "GameFramework/Pawn.h"
//#include "GameFramework/Controller.h"
//#include "Kismet/GameplayStatics.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	if (!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			Main = Cast<AMainCharacter>(Pawn);

		}
	}


}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (!Pawn) {
		Pawn = TryGetPawnOwner();
	}
	if(Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
		Main = Cast<AMainCharacter>(Pawn);
		IsJumpKeyPressed = Main->bJumpKey;
		
		//}
		//bIsInAir = !(Pawn->GetMovementComponent()->IsMovingOnGround());
		//bIsSwing = GetMainCharacter()->
	}
}

//bool UMainAnimInstance::IsJumpKeyPressed()
//{
//	
//	Pawn = TryGetPawnOwner();
//	Main = Cast<AMainCharacter>(Pawn);
//	return Main->bJumpKey;
//}

AMainCharacter* UMainAnimInstance::GetMainCharacter() const
{
	return Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}




