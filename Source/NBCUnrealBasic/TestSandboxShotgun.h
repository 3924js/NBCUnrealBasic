// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponSandbox.h"
#include "TestSandboxShotgun.generated.h"

/**
 * 
 */
UCLASS()
class NBCUNREALBASIC_API ATestSandboxShotgun : public AWeaponSandbox
{
	GENERATED_BODY()
public:
	virtual void UseWeapon() override;

};
