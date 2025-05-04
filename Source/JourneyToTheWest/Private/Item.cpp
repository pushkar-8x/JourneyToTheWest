// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Characters/BlackMyth.h"
#include "NiagaraComponent.h"

AItem::AItem()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());

	EmbersFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmbersFx"));
	EmbersFx->SetupAttachment(GetRootComponent());

}



void AItem::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

}


float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if(ItemState==EItemState::EIS_Hovering)
	AddActorWorldOffset(FVector(0, 0, TransformedSin()));

}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherName = OtherActor->GetName();
	ABlackMyth* BlackMyth = Cast<ABlackMyth>(OtherActor);
	if (BlackMyth)
	{
		BlackMyth->SetOverlappingItem(this);
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Black, OtherName);
	}

}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const FString OtherName = FString("Exited item -- ")+ OtherActor->GetName();
	ABlackMyth* BlackMyth = Cast<ABlackMyth>(OtherActor);
	if (BlackMyth)
	{
		BlackMyth->SetOverlappingItem(nullptr);
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Black, OtherName);
	}
}
