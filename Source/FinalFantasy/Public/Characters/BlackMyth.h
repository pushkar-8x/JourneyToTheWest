// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterType.h"
#include "Item.h"
#include "BlackMyth.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class FINALFANTASY_API ABlackMyth : public ACharacter
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

	void Attack();
	bool CanDisarm();
	bool CanArm();
	void PlayAttackMontage();

	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
		void AttackEnds();

	UFUNCTION(BlueprintCallable)
		void Disarm();

	UFUNCTION(BlueprintCallable)
		void Arm();

	UFUNCTION(BlueprintCallable)
		void FinishEquip();

	void OnEKeyPressed();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleInstanceOnly)
	 AItem* OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, Category = "Weapon")
		AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly , Category = Montages)
		UAnimMontage* AttackMontage;
	
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
