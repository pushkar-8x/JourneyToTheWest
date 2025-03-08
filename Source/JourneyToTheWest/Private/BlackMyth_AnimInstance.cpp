// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackMyth_AnimInstance.h"
#include "Characters/BlackMyth.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlackMyth_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ABlackMyth>(TryGetPawnOwner());

	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void UBlackMyth_AnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		bIsFalling = CharacterMovement->IsFalling();
		CharacterState = Character->GetCharacterState();
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::White, UEnum::GetValueAsString(CharacterState));
	}
}

