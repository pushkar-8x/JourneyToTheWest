// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "Widget_HealthBar.generated.h"


/**
 * 
 */
UCLASS()
class JOURNEYTOTHEWEST_API UWidget_HealthBar : public UWidgetComponent
{
	GENERATED_BODY()

public :
	void SetHealthPercentage(float Percentage);

private:
	UPROPERTY()
	class UHealthBar* HealthBarWidget;
	
};
