// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectible.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/BlackMyth.h"

void ACollectible::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABlackMyth* BlackMyth = Cast<ABlackMyth>(OtherActor);
	if (BlackMyth)
	{
		if (PickupSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}
		Destroy();
	}
}
