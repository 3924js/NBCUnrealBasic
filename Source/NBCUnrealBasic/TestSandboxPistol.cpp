// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSandboxPistol.h"

void ATestSandboxPistol::UseWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing the weapon, %s"), *GetName());
	if (!GetWorld()->GetTimerManager().IsTimerActive(FireTimerHandle)) {
		FVector Start = GetOwner()->GetActorLocation();

		FHitResult HitResult = LinetraceOneShot(GetOwner()->GetActorLocation(), GenerateDirection());
		if (IsValid(HitResult.GetActor())) {
			GenerateDamage(HitResult);
			Fire();
		}
		
	}
}
