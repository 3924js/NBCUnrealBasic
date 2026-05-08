// Copyright Epic Games, Inc. All Rights Reserved.

#include "NBCUnrealBasicCharacter.h"
#include "WeaponBase.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ANBCUnrealBasicCharacter

ANBCUnrealBasicCharacter::ANBCUnrealBasicCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ANBCUnrealBasicCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ANBCUnrealBasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANBCUnrealBasicCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANBCUnrealBasicCharacter::Look);

		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ANBCUnrealBasicCharacter::Fire);

		// Aiming
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &ANBCUnrealBasicCharacter::ZoomIn);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Completed, this, &ANBCUnrealBasicCharacter::ZoomOut);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANBCUnrealBasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	if (IsValid(WeaponClass)) {
		UE_LOG(LogTemp, Warning, TEXT("Spawning Weapon"));
		FActorSpawnParameters Params;
		Params.Owner = this;
		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorTransform(), Params);
		if (IsValid(CurrentWeapon)) {
			UE_LOG(LogTemp, Warning, TEXT("Spawning Weapon"));
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
		
	}
	TargetRecoil = 0.0f;
	CurrentRecoil = 0.0f;
	OriginalZoomDistance = CameraBoom->TargetArmLength;
	OriginalFOV = FollowCamera->FieldOfView;
}

void ANBCUnrealBasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!FMath::IsNearlyZero(CurrentRecoil) || !FMath::IsNearlyZero(TargetRecoil)) {
		float PrevRecoil = CurrentRecoil;
		if (FMath::IsNearlyZero(TargetRecoil)) {
			CurrentRecoil = FMath::FInterpTo(CurrentRecoil, TargetRecoil, DeltaTime, RecoilRecoveryRate);
		}
		else {
			CurrentRecoil = FMath::FInterpTo(CurrentRecoil, TargetRecoil, DeltaTime, RecoilApplyRate);
		}
		if (FMath::IsNearlyEqual(CurrentRecoil, TargetRecoil, 1.0f)) TargetRecoil = 0;
		AddControllerPitchInput(PrevRecoil - CurrentRecoil);
	}
}

void ANBCUnrealBasicCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ANBCUnrealBasicCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANBCUnrealBasicCharacter::Fire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Fire Input!"));
	if (IsValid(CurrentWeapon)) {
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon exists!"));
		CurrentWeapon->UseWeapon();
		if (CurrentWeapon->HasRecoil()) ApplyRecoil();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("CurrentWeapon not valid!"));
	}
	
}

void ANBCUnrealBasicCharacter::ApplyRecoil()
{
	if (!IsValid(CurrentWeapon) ) return;
	if (!CurrentWeapon->HasRecoil()) return;
	TargetRecoil += CurrentWeapon->GetRecoilPerShot();
}

void ANBCUnrealBasicCharacter::ZoomIn(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ZoomIn Called!"));
	CameraBoom->TargetArmLength *= ZoomDistanceRatio;
	FollowCamera->FieldOfView *= ZoomFOVRatio;
}

void ANBCUnrealBasicCharacter::ZoomOut(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ZoomOut Called!"));
	CameraBoom->TargetArmLength = OriginalZoomDistance;
	FollowCamera->FieldOfView = OriginalFOV;
}