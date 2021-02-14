// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "War_ArmoredCharacter.h"
#include "War_Armored.h"
#include "War_ArmoredProjectile.h"
#include "Dummy.h"
#include "MaterialAtravezable.h"
#include "EscudoPlano.h"
#include "EscudoBurbuja.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"

//#include "Components/TimelineComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AWar_ArmoredCharacter

AWar_ArmoredCharacter::AWar_ArmoredCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(40.f, 94.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(false);
	Mesh1P->SetupAttachment(GetCapsuleComponent());
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	EscudoP = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldFlatMesh"));
	EscudoP->SetOnlyOwnerSee(false);
	EscudoP->SetupAttachment(Mesh1P);
	EscudoP->SetupAttachment(Mesh1P, TEXT("ShieldF_Socket"));
	EscudoP->bCastDynamicShadow = false;
	EscudoP->CastShadow = false;
	EscudoP->RelativeLocation = FVector(15.f, 0.f, 0.f);

	EscudoB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldBubleMesh"));
	EscudoB->SetOnlyOwnerSee(false);
	EscudoB->SetupAttachment(Mesh1P);
	EscudoB->SetupAttachment(Mesh1P, TEXT("ShieldB_Socket"));
	EscudoB->bCastDynamicShadow = false;
	EscudoB->CastShadow = false;
	EscudoB->RelativeLocation = FVector(0.f, 0.f, 90.f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(Mesh1P);
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetupAttachment(Mesh1P, TEXT("HeadSocket"));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(Mesh1P, TEXT("Weapon_Socket"));
	FP_Gun->SetupAttachment(GetCapsuleComponent());

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetupAttachment(Mesh1P, TEXT("Bullet_Socket"));
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 50.f, 0));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(65.0f, -22.0f, -24.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	//Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);
}

void AWar_ArmoredCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	DamageAmount = 0;

	HPC = 1000.0f;
	FullHealth = 1000.0f;
	Health = FullHealth;
	HealthPercentage = 1.0f;
	bCanBeDamaged = true;

	FullEnergy = 1000.0f;
	Energy = 0;
	EnergyPercentage = 0.0f;
	PreviousEnergy = EnergyPercentage;
	EnergyValue = 0.0f;
	bCanUseEnergy = true;

	FullArmor = 0.8f;
	Armor = 0;
	ArmorPercentage = 0.0f;
	bCanUseArmor = false;

	FullDamage = 10.0f;
	Damage = 2500.0f;
	DamagePercentage = 50.65f;

	compraRebote = 100.0f;
	compraAtravezar = 200.0f;
	compraReduccion = 150.0f;
	compraRobo = 250.0f;
	compraSeguimiento = 150.0f;

	if (EnergyCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetEnergyValue"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetEnergyState"));

		MyTimeline = NewObject<UTimelineComponent>(this, FName("Energy Animation"));
		MyTimeline->AddInterpFloat(EnergyCurve, TimelineCallback);
		MyTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		MyTimeline->RegisterComponent();
	}

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Weapon_Socket"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		Mesh1P->SetHiddenInGame(false, true);
	}
}

float AWar_ArmoredCharacter::DamageCharacter(float DamageC)
{
	HealthPercentage -= DamageC;

	return HealthPercentage;
}

float AWar_ArmoredCharacter::DamageAcumulator(float DamageAlator)
{
	DamageAmount -= DamageAlator;
	DamageAmount = DamageAmount / DamageAlator;

	return DamageAmount;
}

float AWar_ArmoredCharacter::EnergyRecopilacion(float EnergyRecompilacion)
{
	EnergyRecompilacion -= EnergyPercentage;

	return EnergyRecompilacion;
}

void AWar_ArmoredCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FirstPersonCameraComponent->SetWorldRotation(GetViewRotation());

	AWar_ArmoredCharacter* Character = Cast<AWar_ArmoredCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	FVector CharacterVelocity = Character->GetCharacterMovement()->Velocity;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SI: %s"), *CharacterVelocity.ToString()));

	//DamagePorCaida(-DamageCaida);

	if (MyTimeline != nullptr)
	{
		MyTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWar_ArmoredCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AWar_ArmoredCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AWar_ArmoredCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWar_ArmoredCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWar_ArmoredCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWar_ArmoredCharacter::LookUpAtRate);
}

void AWar_ArmoredCharacter::Balas(UWorld* const World)
{
	TArray<FHitResult> HitResults;
	HitResults.Init(FHitResult(ForceInit), 10);

	FVector StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	FVector EndTrace = (FirstPersonCameraComponent->GetForwardVector() * 2000.f) + StartTrace;

	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();
	TraceParams->bTraceComplex = false;
	FCollisionResponseParams ResponseParams(ECollisionResponse::ECR_Overlap);

	World->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, ECC_Visibility, *TraceParams, ResponseParams);

	for (int i = 0; i != HitResults.Num(); ++i)
	{
		if (Atravezar == true)
		{
			if (HitResults.Num() > 0)
			{
				CreateBulletHole(&HitResults[0], World);
			}

			AStaticMeshActor* PotencialSolidMaterial = Cast<AStaticMeshActor>(HitResults[i].Actor.Get());

			if (PotencialSolidMaterial != NULL && !PotencialSolidMaterial->IsPendingKill())
			{
				break;
			}

			AMaterialAtravezable* PotentialMaterialAtre = Cast<AMaterialAtravezable>(HitResults[i].Actor.Get());

			if (PotentialMaterialAtre != NULL && !PotentialMaterialAtre->IsPendingKill())
			{
				DamageAmount = DamageAmount / PotentialMaterialAtre->MaterialResistance;
			}

			AWar_ArmoredProjectile* Bullet = Cast<AWar_ArmoredProjectile>(HitResults[i].Actor.Get());

			if (Bullet != NULL && !Bullet->IsPendingKill())
			{
				DamageAcumulator(Bullet->Damage);
			}

			if (this != NULL && !this->IsPendingKill())
			{
				UpdateEnergy(EnergyRecopilacion(Bullet->Damage));
				UpdateHealth(DamageCharacter(DamageAmount));
			}

			DrawDebugLine(World, StartTrace, EndTrace, FColor::Red, false, 5.f);
		}
		else if (Atravezar == false)
		{
			return;
		}
	}
	for (int i = 0; i != HitResults.Num(); ++i)
	{
		if (Robar == true)
		{
			if (HitResults.Num() > 0)
			{
				CreateBulletHole(&HitResults[0], World);
			}
			
			AWar_ArmoredProjectile* Bullet = Cast<AWar_ArmoredProjectile>(HitResults[i].Actor.Get());

			if (Bullet != NULL && !Bullet->IsPendingKill())
			{
				DamageAcumulator(Bullet->Damage);
				UpdateHealth(DamageCharacter(DamageAmount));					
				UpdateEnergy(EnergyRecopilacion(Bullet->Robo));
			}

			if (this != NULL && !this->IsPendingKill())
			{

			}
			DrawDebugLine(World, StartTrace, EndTrace, FColor::Red, false, 5.f);
		}
		else if (Robar == false)
		{
			return;
		}
	}
	for (int i = 0; i != HitResults.Num(); ++i)
	{
		if (Reducir == true)
		{
			if (HitResults.Num() > 0)
			{
				CreateBulletHole(&HitResults[0], World);
			}

			AWar_ArmoredProjectile* Bullet = Cast<AWar_ArmoredProjectile>(HitResults[i].Actor.Get());

			if (Bullet != NULL && !Bullet->IsPendingKill())
			{
				DamageAcumulator(Bullet->Damage);
				UpdateHealth(DamageCharacter(DamageAmount));
				UpdateEnergy(EnergyRecopilacion(Bullet->Reduccion));
			}
			DrawDebugLine(World, StartTrace, EndTrace, FColor::Red, false, 5.f);
		}
		else if (Reducir == false)
		{
			return;
		}
	}
}

void AWar_ArmoredCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL /*&& !FMath::IsNearlyZero(Energy, 0.001f) && bCanUseEnergy*/)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FRotator SpawnRotation = GetViewRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AWar_ArmoredProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			//Balas(World);
		}
		// try and play the sound if specified
		if (FireSound != NULL)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != NULL)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		if (MyTimeline != nullptr) MyTimeline->Stop();
		GetWorldTimerManager().ClearTimer(EnergyTimerHandle);
		SetEnergyChange(-100.0f);
		GetWorldTimerManager().SetTimer(EnergyTimerHandle, this, &AWar_ArmoredCharacter::UpdateEnergyPerS, 5.0f, false);
		//GetWorldTimerManager().SetTimer(TimerHandle_Task, this, &AWar_ArmoredCharacter::OnFire, 0.1f);
	}
}

void AWar_ArmoredCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AWar_ArmoredCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AWar_ArmoredCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AWar_ArmoredCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AWar_ArmoredCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWar_ArmoredCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AWar_ArmoredCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AWar_ArmoredCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AWar_ArmoredCharacter::EndTouch);
		return true;
	}
	return false;
}

void AWar_ArmoredCharacter::CreateBulletHole(FHitResult * Object, UWorld* const World)
{
	if (BulletHoleDecal != NULL)
	{
		FVector BulletHoleSize = FVector(5.5f, 10.f, 10.f);
		UGameplayStatics::SpawnDecalAtLocation(World, BulletHoleDecal, BulletHoleSize, Object->ImpactPoint, Object->ImpactNormal.Rotation(), 10.f);
	}
}

FRotator AWar_ArmoredCharacter::GetViewRotation() const
{
	if (Controller)
	{
		return Controller->GetControlRotation();
	}

	return FRotator(RemoteViewPitch / 255.f * 360.f, GetActorRotation().Yaw, 0.f);
}

float AWar_ArmoredCharacter::CalculateFallingDamage(float mass, float altura)
{
	float TotalDamage;
	float Altura;
	float ImpactVel;
	float Distancia;
	float EnergyC;
	float Gravedad = 9.80665;
	float GxAltura;
	float MassxImpactVel;
	float FuerzaFinal;

	Altura = altura;
	Distancia = Altura;
	GxAltura = (Gravedad * Altura);
	GxAltura *= 2;
	ImpactVel = (sqrtf(GxAltura));

	ImpactVel *= ImpactVel;
	MassxImpactVel = (ImpactVel * mass);
	EnergyC = (0.5f * MassxImpactVel);

	FuerzaFinal = (EnergyC);
	TotalDamage = (FuerzaFinal / 50);

	return TotalDamage;
}

float AWar_ArmoredCharacter::GetHealth()
{
	return HealthPercentage;
}

float AWar_ArmoredCharacter::GetEnergy()
{
	return EnergyPercentage;
}

float AWar_ArmoredCharacter::GetArmor()
{
	return ArmorPercentage;
}

float AWar_ArmoredCharacter::GetDamage()
{
	return DamagePercentage;
}

FText AWar_ArmoredCharacter::GetHealthIntText()
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);

	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);

	return HPText;
}

FText AWar_ArmoredCharacter::GetEnergyIntText()
{
	int32 EP = FMath::RoundHalfFromZero(EnergyPercentage * 1000);

	FString EPS = FString::FromInt(EP);
	FString FullEPS = FString::FromInt(FullEnergy);
	FString EnergyHUD = EPS + FString(TEXT("/")) + FullEPS;
	FText EPText = FText::FromString(EnergyHUD);

	return EPText;
}

FText AWar_ArmoredCharacter::GetArmorIntText()
{
	int32 AP = FMath::RoundHalfFromZero(ArmorPercentage * 100);

	FString APS = FString::FromInt(AP);
	FString ArmorHUD = APS + FString(TEXT("%"));
	FText APText = FText::FromString(ArmorHUD);

	return APText;
}

void AWar_ArmoredCharacter::SetDamageState()
{
	bCanBeDamaged = true;
}

void AWar_ArmoredCharacter::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AWar_ArmoredCharacter::SetDamageState, 2.0f, false);
}

void AWar_ArmoredCharacter::SetEnergyValue()
{
	TimelineValue = MyTimeline->GetPlaybackPosition();
	CurveFloatValue = PreviousEnergy + EnergyValue * EnergyCurve->GetFloatValue(TimelineValue);

	Energy = FMath::Clamp(CurveFloatValue*FullHealth, 0.0f, FullEnergy);
	EnergyPercentage = FMath::Clamp(CurveFloatValue, 0.0f, 1.0f);
}

void AWar_ArmoredCharacter::SetEnergyState()
{
	bCanUseEnergy = true;
	EnergyValue = 0.0;

	if (GunDefaultMaterial)
	{
		FP_Gun->SetMaterial(0, GunDefaultMaterial);
	}
}

bool AWar_ArmoredCharacter::PlayFlash()
{
	if (redFlash)
	{
		redFlash = false;
		return true;
	}

	return false;
}

float AWar_ArmoredCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	bCanBeDamaged = false;
	redFlash = true;
	UpdateHealth(-DamageAmount);
	UpdateEnergy(DamageAmount);

	if (bCanUseArmor == false)
	{
		UpdateHealth((-DamageAmount) / ArmorPercentage);
	}

	return DamageAmount;
}

void AWar_ArmoredCharacter::UpdateHealth(float HealthChange)
{
	Health = FMath::Clamp(Health += HealthChange, 0.0f, FullHealth);
	HealthPercentage = Health / FullHealth;
}

void AWar_ArmoredCharacter::UpdateEnergy(float EnergyChange)
{
	float GEnergy = (rand() / (float)RAND_MAX * 0.06f);
	Energy = FMath::Clamp(Energy += EnergyChange, 5.0f, FullEnergy);
	EnergyPercentage = (Energy / FullEnergy) + GEnergy;
}

void AWar_ArmoredCharacter::UpdateArmor(float ArmorChange)
{
	Armor = FMath::Clamp(Armor += ArmorChange, 0.0f, FullArmor);
	ArmorPercentage = Armor + FullArmor;
}

float AWar_ArmoredCharacter::UpdateDamage(float DamageChange)
{
	float DamageActual = 0.0f;
	Damage = FMath::Clamp(Damage += DamageChange, 2.0f, FullDamage);
	DamagePercentage = Damage / FullDamage;
	
	return DamagePercentage;
}

void AWar_ArmoredCharacter::UpdateEnergyPerS()
{
	/*PreviousEnergy = EnergyPercentage;
	EnergyPercentage = Energy / FullEnergy;
	EnergyValue = 1.0f;

	if (MyTimeline != nullptr)
	{
		MyTimeline->PlayFromStart();
	}*/
}

void AWar_ArmoredCharacter::SetEnergyChange(float MagicChange)
{
	bCanUseEnergy = false;
	PreviousEnergy = EnergyPercentage;
	EnergyValue = (MagicChange / FullEnergy);

	if (GunOverheatMaterial)
	{
		FP_Gun->SetMaterial(0, GunOverheatMaterial);
	}

	if (MyTimeline != nullptr)
	{
		MyTimeline->PlayFromStart();
	}
}
