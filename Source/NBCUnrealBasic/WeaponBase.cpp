// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void AWeaponBase::UseWeapon(FVector FireLocation, FVector FireVector)
{
	UE_LOG(LogTemp, Warning, TEXT("Firing the weapon, %s"), *GetName());
	FHitResult HitResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(Owner);
	float SpreadAngle = FMath::Atan(MOA * 0.02908f / 100.f);
	for (int i = 0; i < BulletsOnSignleFire; i++) {
		FVector ShootDirection = FMath::VRandCone(FireVector, SpreadAngle);
		bool bHit = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			FireLocation,
			FireLocation + ShootDirection * MaxDistance,
			UEngineTypes::ConvertToTraceType(ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.0f);
		if (bHit && IsValid(HitResult.GetActor())) {
			float DamageToApply = HitResult.Distance < (MaxDamageDistance) ?
				DamagePerBullet :
				FMath::Max(0, DamagePerBullet - DamageAttenuationRate * HitResult.Distance);
			UE_LOG(LogTemp, Warning, TEXT("Applying %f Damage to %s !"), DamageToApply, *HitResult.GetActor()->GetName());

			
			float temp = UGameplayStatics::ApplyDamage(HitResult.GetActor(), DamageToApply, Cast<APawn>(Owner)->GetController(), Owner, UDamageType::StaticClass());
			UE_LOG(LogTemp, Warning, TEXT("returned damage to %f !"), temp);
		}
	}
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}


