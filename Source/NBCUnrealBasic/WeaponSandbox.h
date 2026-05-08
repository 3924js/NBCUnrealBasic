// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "WeaponSandbox.generated.h"

/**
 * 
 */
UCLASS()
class NBCUNREALBASIC_API AWeaponSandbox : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void UseWeapon() override;
protected:
	virtual void Fire() override;
	virtual void HandleFireDelay() override;
	virtual void PlayEffects();
	virtual FVector GenerateDirection();
	virtual FHitResult LinetraceOneShot(FVector Start, FVector Direction);
	virtual void GenerateDamage(FHitResult Result);
	virtual void GenerateDamage(AActor* Target, FHitResult Result);
	
};
