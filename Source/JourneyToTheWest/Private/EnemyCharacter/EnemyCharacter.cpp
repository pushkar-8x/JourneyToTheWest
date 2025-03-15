// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Debug/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::GetHit(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint);
	DRAW_SPHERE_COLOR(ImpactPoint, FColor::Yellow);
	SetDirectionalHitReaction(ImpactPoint);
	if (HitSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundEffect, ImpactPoint) ;
	}
}

void AEnemyCharacter::SetDirectionalHitReaction(const FVector& ImpactPoint)
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

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60.f,
		5.f, FColor::Red, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + HitVector * 60.f,
		5.f, FColor::Blue, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f,
		5.f, FColor::Blue, 5.f);
}

