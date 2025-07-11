// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);
}

float UAttributeComponent::GetHealthPercentage()
{
	return CurrentHealth/MaxHealth;
}

bool UAttributeComponent::IsAlive()
{
	return CurrentHealth > 0.f;
}

