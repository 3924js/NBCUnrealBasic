// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"

#include "WeaponBase.generated.h"

class UArrowComponent;
class UNiagaraComponent;
UCLASS()
class NBCUNREALBASIC_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable)
	bool HasRecoil();
	UFUNCTION(BlueprintCallable)
	float GetRecoilPerShot();
	UFUNCTION(BlueprintCallable)
	virtual void UseWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<class UArrowComponent> FirePoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	//Effects
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UAudioComponent> FireSFX;		//Damage to apply per bullet.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UNiagaraComponent> FireParticle;		//Damage to apply per bullet.

	//Shot Trace
	UPROPERTY(BlueprintReadWrite)
	bool CanFire;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float MOA;				//How much is concentrated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float MaxDistance;				//How much is concentrated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bAlwaysShotCenter;	//Whether to always shot the aiming point at the first shot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bIsLineTracing;	//Whether using linetrace or sweeptrace
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bUsingGravity;		//whether it's affected by gravity, not availiable when linetracing

	//Automaic timer
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float RoF;				//Rate of Fire
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
	FTimerHandle FireTimerHandle;

	//Ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 AmmoPerFire;	//How many bullets to fire on a single shot
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Weapon)
	int32 CurrentAmmo;			//How many ammos are in the mag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	int32 MaxAmmo;				//How many ammos to have in a single mag
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AActor> AmmoClass;	//Bullet actor to use when not linetracing.
	
	//Recoil
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRecoil;		//whether to has recoil
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float RecoilPerShot;		//How much to recoil

	//Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float DamagePerBullet;		//Damage to apply per bullet.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float DamageAttenuationRate;		//how much to decrease damage by distance.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float MaxDamageDistance;		//where the others to start taking maximum damage.

	

	UFUNCTION(BlueprintCallable)
	virtual void Fire();
	UFUNCTION(BlueprintCallable)
	virtual void HandleFireDelay();
	
};
