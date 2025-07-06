// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Characters/BlackMyth.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"
#include "NiagaraComponent.h"


AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
		ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex , bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	//GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::White, GetOwner()->GetName());
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}
	
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(this, Start, End , FVector(10.f,10.f,10.f),
		BoxTraceStart->GetComponentRotation() ,
		ETraceTypeQuery::TraceTypeQuery1 ,true , ActorsToIgnore ,
		EDrawDebugTrace::None , BoxHit , true );

	
	if (BoxHit.GetActor())
	{
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::White, BoxHit.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(),
			WeaponDamage,
			GetInstigator()->GetController(), this, UDamageType::StaticClass()

		);
		IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		}
		IgnoreActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::White, FString("Nothing detected!"));
	}

	
}



void AWeapon::Equip(USceneComponent* InParent, FName InSocketName , AActor* NewOwner , APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	ItemState = EItemState::EIS_Equipped;
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}

	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (EmbersFx)
	{
		EmbersFx->Deactivate();
	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}


