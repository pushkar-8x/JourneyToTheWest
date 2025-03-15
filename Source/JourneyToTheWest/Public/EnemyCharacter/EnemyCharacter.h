// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class JOURNEYTOTHEWEST_API AEnemyCharacter : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	/**
	*Animation Montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* HitSoundEffect;
protected:

	virtual void BeginPlay() override;

	void PlayHitReactMontage(const FName& SectionName);

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit(const FVector& ImpactPoint) override;
	void SetDirectionalHitReaction(const FVector& ImpactPoint);
};
