// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
UCLASS()
class JOURNEYTOTHEWEST_API ABreakableActor : public AActor , public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActor();
	virtual void Tick(float DeltaTime) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		UGeometryCollectionComponent* GeometryCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCapsuleComponent* Capsule;
private:


	UPROPERTY(EditAnywhere , Category = "Breakable Attributes")
		TArray<TSubclassOf<class ACollectible>> CollectibleClasses;

	bool bIsBroken;
};
