// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterType.h"
#include "Item.h"
#include "BaseCharacter.h"
#include "BlackMyth.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AItem;

UCLASS()
class JOURNEYTOTHEWEST_API ABlackMyth : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlackMyth();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);

	virtual void Attack() override;
	virtual void Die() override;
	virtual bool CanAttack() override;
	bool CanDisarm();
	bool CanArm();
	void PlayAttackMontage();

	void PlayEquipMontage(FName SectionName);

	virtual void AttackEnds() override;

	UFUNCTION(BlueprintCallable)
		void ForwardCombo();

	UFUNCTION(BlueprintCallable)
		void Disarm();

	UFUNCTION(BlueprintCallable)
		void Arm();

	UFUNCTION(BlueprintCallable)
		void FinishEquip();

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void OnEKeyPressed();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleInstanceOnly)
	 AItem* OverlappingItem;


	UPROPERTY(VisibleAnywhere)
		int32 attackComboIndex = 0;

	

	
	UPROPERTY(EditDefaultsOnly , Category = Montages)
		UAnimMontage* EquipMontage;

	UPROPERTY(BluePrintReadWrite, meta = (AllowPrivateAccess = true))
		EActionState ActionState = EActionState::EAS_Unoccupied;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE EActionState GetActionState() { return ActionState; }
	FORCEINLINE void SetOverlappingItem(AItem* Item) { this->OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() { return CharacterState; }

};
