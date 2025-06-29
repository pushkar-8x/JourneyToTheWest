// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Characters/CharacterType.h"
#include "EnemyCharacter.generated.h"


class UAttributeComponent;
class UWidgetComponent;

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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* HitSoundEffect;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
		UParticleSystem* HitFx;

	UPROPERTY()
		AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
		float CombatRadius = 500.f;

private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
		class UWidget_HealthBar* HealthBarComponent;

protected:

	virtual void BeginPlay() override;
	void Die();
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(BlueprintReadOnly)
		EDeathPose DeathPose = EDeathPose::EDP_Alive;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint);
	void SetDirectionalHitReaction(const FVector& ImpactPoint);

	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
