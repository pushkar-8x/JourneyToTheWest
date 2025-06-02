// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Widget_HealthBar.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UWidget_HealthBar::SetHealthPercentage(float Percentage)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthBarWidget && HealthBarWidget->HealthBar)
	{
		HealthBarWidget->HealthBar->SetPercent(Percentage);
	}
}
