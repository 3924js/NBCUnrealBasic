// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyPawn.generated.h"

class UCapsuleComponent;
class USphereComponent;
UCLASS()
class NBCUNREALBASIC_API AEnemyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemyPawn)
	UCapsuleComponent* CapsuleComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemyPawn)
	USkeletalMeshComponent* MeshComp;

	//to search player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = EnemyPawn)
	USphereComponent* RecognitionRange;

	FTimerHandle SearchTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = EnemyPawn)
	float SearchInterval;
	AActor* Target;
	void CheckTargetVisible();
	void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);
	FTraceDelegate TraceDelegate;

	UFUNCTION()
	void OnRecogRangeEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRecogRangeExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
