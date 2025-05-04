// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Collectible.generated.h"

/**
 * 
 */
UCLASS()
class JOURNEYTOTHEWEST_API ACollectible : public AItem
{
	GENERATED_BODY()

protected:
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

public:

	UPROPERTY(EditAnywhere, Category = Audio)
	USoundBase* PickupSound;

	UPROPERTY(EditAnywhere, Category = "Collectible Properties")
		int32 GoldCount = 5;
	
};
