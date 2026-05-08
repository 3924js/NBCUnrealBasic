// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SandboxWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class NBCUNREALBASIC_API ASandboxWeaponBase : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Fire() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SandboxFire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	bool CheckAmmo();
	
	UFUNCTION(BlueprintCallable)
	void LinetraceOneShot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	void PlaySound(USoundBase* Sound);

	//총알 업데이트 담당
	UFUNCTION(BlueprintCallable)
	void UpdateAmmo();
};
