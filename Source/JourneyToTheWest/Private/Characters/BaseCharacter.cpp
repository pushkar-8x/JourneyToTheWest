// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components//BoxComponent.h"
#include "Weapon.h"
#include "ActorComponents/AttributeComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{
	
}

void ABaseCharacter::PlayEquipMontage(FName SectionName)
{
	
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::AttackEnds()
{

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::Die()
{
	
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::SetDirectionalHitReaction(const FVector& ImpactPoint)
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector HitVector = (ImpactPoint - GetActorLocation()).GetSafeNormal();

	const double CosAngle = FVector::DotProduct(ForwardVector, HitVector);
	double Angle = FMath::Acos(CosAngle);

	Angle = FMath::RadiansToDegrees(Angle);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Angle : %f"), Angle));
	}

	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, HitVector);

	if (CrossProduct.Z < 0.f)
	{
		Angle *= -1.f;
	}

	FName SectionName("FromLeft");
	if (Angle <= 45.f && Angle >= -45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (Angle >= 45.f && Angle <= 135.f)
	{
		SectionName = FName("FromRight");
	}
	else if (Angle <= -45.f && Angle <= -135.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (Angle >= 135.f && Angle <= -135.f)
	{
		SectionName = FName("FromBack");
	}

	PlayHitReactMontage(SectionName);

	/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60.f,
		5.f, FColor::Red, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + HitVector * 60.f,
		5.f, FColor::Blue, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f,
		5.f, FColor::Blue, 5.f);*/
}


