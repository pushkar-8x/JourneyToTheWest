// Fill out your copyright notice in the Description page of Project Settings.


#include "Bird.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABird::ABird()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	RootComponent = Capsule;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	Mesh->SetupAttachment(RootComponent);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);


	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::MoveForward(float Value)
{
	if (Controller && (Value != 0))
	{
		FVector Forward = GetActorForwardVector();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor :: Black
				, FString("Calling this moveforward function....") );
		}
		AddMovementInput(Forward, Value);
	}
}

void ABird::Turn(float value)
{
	AddControllerYawInput(value);
}

void ABird::LookUp(float value)
{
	AddControllerPitchInput(value);
}


void ABird::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABird::MoveForward);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABird::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABird::LookUp);

}

