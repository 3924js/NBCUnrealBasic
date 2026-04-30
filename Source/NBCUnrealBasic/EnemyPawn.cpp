// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "NBCUnrealBasicCharacter.h"

// Sets default values
AEnemyPawn::AEnemyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	RecognitionRange = CreateDefaultSubobject<USphereComponent>(TEXT("RecognitionRange"));
	RecognitionRange->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyPawn::BeginPlay()
{
	Super::BeginPlay();
	Target = nullptr;
	TraceDelegate.BindUObject(this, &AEnemyPawn::OnTraceCompleted);
	RecognitionRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn::OnRecogRangeEntered);
	RecognitionRange->OnComponentEndOverlap.AddDynamic(this, &AEnemyPawn::OnRecogRangeExit);
}

void AEnemyPawn::CheckTargetVisible()
{
	UE_LOG(LogTemp, Warning, TEXT("Trace Called!"));
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Single,
		GetActorLocation(),
		Target->GetActorLocation(),
		ECC_Pawn,
		Params,
		FCollisionResponseParams(),
		&TraceDelegate
	);
}

void AEnemyPawn::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	UE_LOG(LogTemp, Warning, TEXT("Trace Completed! %d"), Data.OutHits.Num());
	if (Data.OutHits.Num() == 0) return;

	AActor* HitActor = Data.OutHits[0].GetActor();
	//rotate to player if found
	if (IsValid(HitActor) && HitActor->IsA(ANBCUnrealBasicCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Visible!"));
		FVector Direction = (HitActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		DrawDebugLine(GetWorld(), GetActorLocation(), Target->GetActorLocation(), FColor::Red, false, SearchInterval);
		SetActorRotation(FRotator(0,LookAtRotation.Yaw,0));
	}
}

void AEnemyPawn::OnRecogRangeEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor) && OtherActor->IsA(ANBCUnrealBasicCharacter::StaticClass())) {
		Target = OtherActor;
		GetWorld()->GetTimerManager().SetTimer(SearchTimerHandle, this, &AEnemyPawn::CheckTargetVisible, SearchInterval, true);
		CheckTargetVisible();
		UE_LOG(LogTemp, Warning, TEXT("Target Found!"));
	}
}

void AEnemyPawn::OnRecogRangeExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor) && OtherActor->IsA(ANBCUnrealBasicCharacter::StaticClass())) {
		Target = nullptr;
		if (GetWorld()->GetTimerManager().IsTimerActive(SearchTimerHandle)) {
			GetWorld()->GetTimerManager().ClearTimer(SearchTimerHandle);
		}
		UE_LOG(LogTemp, Warning, TEXT("Target Lost!"));
	}
}

float AEnemyPawn::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("TakeDamage called! Amount: %f"), DamageAmount);

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	UE_LOG(LogTemp, Warning, TEXT("ActualDamage after Super: %f"), ActualDamage);

	UE_LOG(LogTemp, Warning, TEXT("%s is taking %f damage from %s"),
		*GetName(),
		DamageAmount,
		IsValid(DamageCauser) ? *DamageCauser->GetName() : TEXT("null")
	);
	return 11;
}

// Called to bind functionality to input
void AEnemyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

