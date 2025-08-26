// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Characters/CharacterType.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h" // <-- for FPathFollowingResult
#include "EnemyCharacter.generated.h"


class UAttributeComponent;
class UWidgetComponent;
//class UPawnSensingComponent* PawnSensing;

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
		float CombatRadius = 2000.f;


	UPROPERTY(EditAnywhere)
		float AttackRadius = 200.f;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		AActor* CurrentPatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		double StoppingDistance = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitTimeMin;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float WaitTimeMax;



private:
	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
		class UWidget_HealthBar* HealthBarComponent;

	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	bool bIsMoving = false;
	int32 currentTargetIndex = 0;

	FTimerHandle PatrolWaitTimer;

	void PatrolWaitOver();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	void SetupCombatTarget(APawn* SeenPawn);

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

protected:

	virtual void BeginPlay() override;
	void Die();

	bool InTargetRange(AActor* Target, double Radius);
	void PlayHitReactMontage(const FName& SectionName);

	void MoveToTarget(AActor* PatrolTarget);

	UPROPERTY(BlueprintReadOnly)
		EDeathPose DeathPose = EDeathPose::EDP_Alive;

public:	
	virtual void Tick(float DeltaTime) override;
	void CheckCombatTarget();
	void UpdatePatrolTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint);
	void SetDirectionalHitReaction(const FVector& ImpactPoint);
	// EnemyCharacter.h



	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
