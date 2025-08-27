// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Characters/CharacterType.h"
#include "Characters/BaseCharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h" // <-- for FPathFollowingResult
#include "EnemyCharacter.generated.h"



class UWidgetComponent;
//class UPawnSensingComponent* PawnSensing;

UCLASS()
class JOURNEYTOTHEWEST_API AEnemyCharacter : public ABaseCharacter 
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	

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
	virtual void Die() override;
	virtual void Attack() override;
	virtual void AttackEnds() override;
	bool InTargetRange(AActor* Target, double Radius);
	

	void MoveToTarget(AActor* PatrolTarget);

	UPROPERTY(BlueprintReadOnly)
		EDeathPose DeathPose = EDeathPose::EDP_Alive;

public:	
	virtual void Tick(float DeltaTime) override;
	void CheckCombatTarget();
	void UpdatePatrolTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	// EnemyCharacter.h



	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
};
