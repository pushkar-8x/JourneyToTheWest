// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOURNEYTOTHEWEST_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()


protected:

	virtual void BeginPlay() override;

public:	

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere , Category =  "Actor Properties")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Actor Properties")
		float MaxHealth;

		
};
