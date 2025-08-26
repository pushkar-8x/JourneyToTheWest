// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter/EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Debug/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ActorComponents/AttributeComponent.h"
#include "Components/WidgetComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "HUD/Widget_HealthBar.h"
#include "Navigation/PathFollowingComponent.h" // <-- for FPathFollowingResult

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	GetMesh()->SetGenerateOverlapEvents(true);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarComponent = CreateDefaultSubobject<UWidget_HealthBar>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercentage(1.0f);
		HealthBarComponent->SetVisibility(false);
	}
	EnemyController = Cast<AAIController>(GetController());
	bIsMoving = false;
	currentTargetIndex = 0;
	CurrentPatrolTarget = PatrolTargets[currentTargetIndex];

	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyCharacter::PawnSeen);
	}
	
}
 
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inside patrolling logic"));
		MoveToTarget(CurrentPatrolTarget);
		if (InTargetRange(CurrentPatrolTarget, StoppingDistance))
		{
			UpdatePatrolTarget();
			UE_LOG(LogTemp, Warning, TEXT("Updated patrol target !"));
		}
	}





}

void AEnemyCharacter::CheckCombatTarget()
{
	if (CombatTarget && HealthBarComponent && EnemyController)
	{
		bool inRange = InTargetRange(CombatTarget, CombatRadius);
		HealthBarComponent->SetVisibility(inRange);

		if (!inRange)
		{
			EnemyController->StopMovement();
			if (UPathFollowingComponent* PFC = EnemyController->GetPathFollowingComponent())
			{
				PFC->AbortMove(*this, FPathFollowingResultFlags::ForcedScript | FPathFollowingResultFlags::NewRequest);
			}
			EnemyState = EEnemyState::EES_Patrolling;			
			CombatTarget = nullptr; 
			UpdatePatrolTarget();
			return;
		}

		if (InTargetRange(CombatTarget, AttackRadius))
		{
			// Start Attacking
			bIsMoving = false;
			EnemyState = EEnemyState::EES_Attacking;
		}
		else
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
		}
	}
}



void AEnemyCharacter::UpdatePatrolTarget()
{
	currentTargetIndex++;
	currentTargetIndex = currentTargetIndex % PatrolTargets.Num();
	CurrentPatrolTarget = PatrolTargets[currentTargetIndex];

	const float WaitTime = FMath::RandRange(WaitTimeMin, WaitTimeMax);
	GetWorldTimerManager().SetTimer(PatrolWaitTimer, this, &AEnemyCharacter::PatrolWaitOver, WaitTime);
}

void AEnemyCharacter::MoveToTarget(AActor* PatrolTarget)
{
	if (EnemyController && PatrolTarget && !bIsMoving)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PatrolTarget);
		MoveRequest.SetAcceptanceRadius(15.f);
		FNavPathSharedPtr NavPath;

		EnemyController->MoveTo(MoveRequest, &NavPath);
		bIsMoving = true;
		//TArray<FNavPathPoint> Path = NavPath->GetPathPoints();
		//for (auto& Point : Path)
		//{
		//	const FVector& Location = Point.Location;
		//	DrawDebugSphere(GetWorld(), Location, 12.f, 12, FColor::Green, false, 10.f);
		//}

	}
}


void AEnemyCharacter::PatrolWaitOver()
{
	bIsMoving = false;
	GetCharacterMovement()->MaxWalkSpeed = 225.f;
	UE_LOG(LogTemp, Warning, TEXT("Finished patrolling !"));
	
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

void AEnemyCharacter::GetHit_Implementation(const FVector& ImpactPoint)
{
	//DRAW_SPHERE(ImpactPoint);
	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Yellow);

	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(true);
	}
	if (Attributes && Attributes->IsAlive())
	{
		SetDirectionalHitReaction(ImpactPoint);
	}
	else
	{
		Die();
	}

	if (HitSoundEffect)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSoundEffect, ImpactPoint) ;
	}
	if (HitFx)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(), HitFx, ImpactPoint
		);
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

	/*UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ForwardVector * 60.f,
		5.f, FColor::Red, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + HitVector * 60.f,
		5.f, FColor::Blue, 5.f);

	UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f,
		5.f, FColor::Blue, 5.f);*/
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarComponent)
	{
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarComponent->SetHealthPercentage(Attributes->GetHealthPercentage());
		CombatTarget = EventInstigator->GetPawn();
		SetupCombatTarget((APawn*)CombatTarget);
	}
		

	return DamageAmount;
}

void AEnemyCharacter::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 Selection = FMath::RandRange(1, 3);
		FString SectionString = FString("Death") + FString::FromInt(Selection);
		FName SectionName(*SectionString);
		//FName SectionName = "Death1";
		DeathPose = static_cast<EDeathPose>(Selection);
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}

	if (HealthBarComponent)
	{
		HealthBarComponent->SetVisibility(false);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.0f);
}

bool AEnemyCharacter::InTargetRange(AActor* Target, double Radius)
{
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemyCharacter::PawnSeen(APawn* SeenPawn)
{
	SetupCombatTarget(SeenPawn);
}

void AEnemyCharacter::SetupCombatTarget(APawn* SeenPawn)
{

	if (EnemyState == EEnemyState::EES_Chasing) return;


	if (SeenPawn->ActorHasTag(FName("Wukong")))
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(PatrolWaitTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;
		bIsMoving = false;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Seen Pawn! Now chasing!"));
	}

}

