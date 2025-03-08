// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterType.h"
#include "BlackMyth_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class JOURNEYTOTHEWEST_API UBlackMyth_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BluePrintReadOnly)
	class ABlackMyth* Character;

	UPROPERTY(BluePrintReadOnly , Category = "Movement")
	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(BluePrintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BluePrintReadOnly, Category = "Movement")
	bool bIsFalling;

	UPROPERTY(BluePrintReadOnly , Category = "Movement")
	ECharacterState CharacterState;

};
