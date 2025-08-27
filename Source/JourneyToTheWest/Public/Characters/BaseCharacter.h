// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class JOURNEYTOTHEWEST_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual bool CanAttack();
	virtual void Die();

	virtual void PlayEquipMontage(FName SectionName);
	void PlayHitReactMontage(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnds();

	UPROPERTY(VisibleInstanceOnly, Category = "Weapon")
	AWeapon* EquippedWeapon;

	/**
*Animation Montages
*/

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	void SetDirectionalHitReaction(const FVector& ImpactPoint);

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = Audio)
	USoundBase* HitSoundEffect;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UParticleSystem* HitFx;

};
