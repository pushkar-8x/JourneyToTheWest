// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
class USoundBase;

UCLASS()
class FINALFANTASY_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	void Equip(USceneComponent* InParent , FName InSocketName);
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
protected:
	
		virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult) override;

	
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		) override;


private:

	UPROPERTY(EditAnywhere, Category = "Weapon Specifics")
		USoundBase* EquipSound;
	
};


