// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSandbox.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

void AWeaponSandbox::UseWeapon()
{
	
}

void AWeaponSandbox::Fire()
{
	CanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AWeaponSandbox::HandleFireDelay, 1.f / RoF, false);
}


void AWeaponSandbox::HandleFireDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
	CanFire = true;
}

FHitResult AWeaponSandbox::LinetraceOneShot(FVector Start, FVector Direction)
{
	FHitResult HitResult(ForceInit);
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Owner);

	FVector End = Start + Direction * MaxDistance;

	bool bHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		2.0f);
	return HitResult;
}

void AWeaponSandbox::GenerateDamage(FHitResult Result)
{
	float DamageToApply = Result.Distance < (MaxDamageDistance) ?
		DamagePerBullet :
		FMath::Max(0, DamagePerBullet - DamageAttenuationRate * Result.Distance);
	UE_LOG(LogTemp, Warning, TEXT("Applying %f Damage to %s !"), DamageToApply, *Result.GetActor()->GetName());


	float temp = UGameplayStatics::ApplyDamage(Result.GetActor(), DamageToApply, Cast<APawn>(Owner)->GetController(), Owner, UDamageType::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("returned damage to %f !"), temp);
}

void AWeaponSandbox::GenerateDamage(AActor* Target, FHitResult Result)
{
	float DamageToApply = Result.Distance < (MaxDamageDistance) ?
		DamagePerBullet :
		FMath::Max(0, DamagePerBullet - DamageAttenuationRate * Result.Distance);
	UE_LOG(LogTemp, Warning, TEXT("Applying %f Damage to %s !"), DamageToApply, *Result.GetActor()->GetName());


	float temp = UGameplayStatics::ApplyDamage(Target, DamageToApply, Cast<APawn>(Owner)->GetController(), Owner, UDamageType::StaticClass());
	UE_LOG(LogTemp, Warning, TEXT("returned damage to %f !"), temp);
}

FVector AWeaponSandbox::GenerateDirection()
{
	float SpreadAngle = FMath::Atan(MOA * 0.02908f / 100.f);
	ACharacter* Character =Cast<ACharacter>(GetOwner());
	FVector OriginalDirection = Character->GetController()->GetControlRotation().Vector();
	FVector ShootDirection = FMath::VRandCone(OriginalDirection, SpreadAngle);
	return ShootDirection;
	UE_LOG(LogTemp, Warning, TEXT("Control Rot: %f %f %f"), OriginalDirection.X, OriginalDirection.Y, OriginalDirection.Z);
}

void AWeaponSandbox::PlayEffects() {
	FireSFX->Play();
	FireParticle->Activate();
}
