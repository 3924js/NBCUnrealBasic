// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxWeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"

void ASandboxWeaponBase::Fire()
{
	SandboxFire();
}

void ASandboxWeaponBase::Reload()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();
}


bool ASandboxWeaponBase::CheckAmmo()
{
	return AmmoPerFire > CurrentAmmo;
}

void ASandboxWeaponBase::LinetraceOneShot(FVector Direction)
{
	FHitResult HitResult(ForceInit);

	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + (Direction * MaxDistance);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{ this, GetOwner() },
		EDrawDebugTrace::ForDuration,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f
		);
}

void ASandboxWeaponBase::PlaySound(USoundBase* Sound) {
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
}

void ASandboxWeaponBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}