// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BlackMyth.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Weapon.h"

// Sets default values
ABlackMyth::ABlackMyth()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);


}

// Called when the game starts or when spawned
void ABlackMyth::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ABlackMyth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}




void ABlackMyth::MoveForward(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (value != 0))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator ControllerYaw(0.f, GetControlRotation().Yaw, 0.f);

		const FVector Direction = FRotationMatrix(ControllerYaw).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}

void ABlackMyth::MoveRight(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if (Controller && (value != 0))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator ControllerYaw(0.f, GetControlRotation().Yaw, 0.f);

		const FVector Direction = FRotationMatrix(ControllerYaw).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, value);
	}
}



void ABlackMyth::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABlackMyth::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ABlackMyth::Attack()
{
	if (ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unoccupied)
	{
		PlayAttackMontage();
		
	}
	
}

bool ABlackMyth::CanDisarm()
{
	bool canDisarm = ActionState == EActionState::EAS_Unoccupied &&
		CharacterState != ECharacterState::ECS_Unoccupied;
	return canDisarm;
}

bool ABlackMyth::CanArm()
{
	bool canArm = ActionState == EActionState::EAS_Unoccupied &&
		CharacterState == ECharacterState::ECS_Unoccupied && EquippedWeapon;
		return canArm;
}

void ABlackMyth::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 selection = FMath::RandRange(0, 1);

		FName SectionName = FName();
		switch (selection)
		{
		case 0:
			SectionName = "Attack1";
			break;

		case 1:
			SectionName = "Attack2";
			break;

		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
		ActionState = EActionState::EAS_Attacking;
	}
}

void ABlackMyth::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ABlackMyth::AttackEnds()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ABlackMyth::Disarm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ABlackMyth::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ABlackMyth::FinishEquip()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ABlackMyth::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABlackMyth::OnEKeyPressed()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		
		EquippedWeapon = OverlappingWeapon;
		OverlappingItem = nullptr;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("UnEquip"));
			CharacterState = ECharacterState::ECS_Unoccupied;
			ActionState = EActionState::EAS_Equipping;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_Equipping;

		}
	}
}

// Called to bind functionality to input
void ABlackMyth::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABlackMyth::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABlackMyth::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABlackMyth::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABlackMyth::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"),IE_Pressed , this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ABlackMyth::Attack);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &ABlackMyth::OnEKeyPressed);

}

