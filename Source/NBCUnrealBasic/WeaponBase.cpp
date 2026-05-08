// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "Components/ArrowComponent.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(RootComponent);

	FireSFX = CreateDefaultSubobject<UAudioComponent>(TEXT("FireSFX"));
	FireSFX->SetupAttachment(RootComponent);
	FireParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireParticle"));
	FireParticle->SetupAttachment(RootComponent);

	AmmoPerFire = 1;
	CurrentAmmo = 0;
	MaxAmmo = 12;
	RoF = 1.f;
	MOA = 10.f;
	CanFire = true;
	MaxDistance = 1000.f;
	DamagePerBullet = 100.f;
}


// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

void AWeaponBase::Fire() {

}

void AWeaponBase::UseWeapon()
{
}

bool AWeaponBase::HasRecoil()
{
	return false;
}

float AWeaponBase::GetRecoilPerShot()
{
	return 0.0;
}

void AWeaponBase::HandleFireDelay() {

}
