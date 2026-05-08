// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTemplateBase.h"

void AWeaponTemplateBase::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("TemplateFireCalled"));
	if (!CanFire) return;

	if (CheckAmmo()) {
		UE_LOG(LogTemp, Warning, TEXT("TemplateFireProcessing"));
		PlayEffects();
		ProcessFiring();
		Super::Fire();
		return;
	}

}
void AWeaponTemplateBase::Reload_Implementation()
{
	CurrentAmmo = MaxAmmo;
}

bool AWeaponTemplateBase::CheckAmmo_Implementation()
{
	return AmmoPerFire <= CurrentAmmo;
}

void AWeaponTemplateBase::UpdateAmmo_Implementation()
{
	CurrentAmmo -= AmmoPerFire;
}
