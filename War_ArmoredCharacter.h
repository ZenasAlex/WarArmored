// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "War_ArmoredCharacter.generated.h"

class UInputComponent;

UCLASS(config = Game)
class AWar_ArmoredCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* FP_Gun;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* EscudoP;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* EscudoB;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UMotionControllerComponent* L_MotionController;

public:
	AWar_ArmoredCharacter();

	float DamageCharacter(float DamageC);

	float DamageAcumulator(float DamageAlator);

	float EnergyRecopilacion(float EnergyRecompilacion);

protected:

	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AWar_ArmoredProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint32 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compra")
		float compraRebote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compra")
		float compraAtravezar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compra")
		float compraRobo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compra")
		float compraReduccion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compra")
		float compraSeguimiento;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float FullEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float Energy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float EnergyPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float PreviousEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		float EnergyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
		float FullArmor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
		float Armor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
		float ArmorPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float FullDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamagePercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool redFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageAmount;

	UFUNCTION(BlueprintCallable, category = "Damage")
		float CalculateFallingDamage(float mass, float altura);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
		UCurveFloat* EnergyCurve;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageType> BulletDamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
		bool Rebotar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
		bool Atravezar = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
		bool Robar = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
		bool Reducir = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tienda")
		bool Seguir = false;

	float CurveFloatValue;
	float TimelineValue;
	bool bCanUseEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Armor")
	bool bCanUseArmor;

	int32 HPC;

	UTimelineComponent* MyTimeline;

	struct FTimerHandle MemberTimerHandle;
	struct FTimerHandle EnergyTimerHandle;

	/** Get Health */
	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth();

	/** Get Energy */
	UFUNCTION(BlueprintPure, Category = "Energy")
		float GetEnergy();

	UFUNCTION(BlueprintPure, Category = "Armor")
		float GetArmor();

	UFUNCTION(BlueprintPure, Category = "Damage")
		float GetDamage();

	/** Get Health Text */
	UFUNCTION(BlueprintPure, Category = "Health")
		FText GetHealthIntText();

	/** Get Energy Text */
	UFUNCTION(BlueprintPure, Category = "Energy")
		FText GetEnergyIntText();

	UFUNCTION(BlueprintPure, Category = "Armor")
		FText GetArmorIntText();

	/** Damage Timer */
	UFUNCTION()
		void DamageTimer();

	/** Set Damage State */
	UFUNCTION()
		void SetDamageState();

	/** Set Magic Value */
	UFUNCTION()
		void SetEnergyValue();

	/** Set Damage State */
	UFUNCTION()
		void SetEnergyState();

	/** Set Damage State */
	UFUNCTION()
		void SetEnergyChange(float EnergyChance);

	/** Set Damage State */
	UFUNCTION()
		void UpdateEnergyPerS();

	/** Play Flash */
	UFUNCTION(BlueprintPure, Category = "Health")
		bool PlayFlash();

	UPROPERTY(EditAnywhere, Category = "Energy")
		class UMaterialInterface* GunDefaultMaterial;

	UPROPERTY(EditAnywhere, Category = "Energy")
		class UMaterialInterface* GunOverheatMaterial;

	/*UFUNCTION()
	void ReceivePointDamage(float Damage, const class UDamageType * DamageType, FVector HitLocation, FVector HitNormal, class UPrimitiveComponent * HitComponent, FName BoneName, FVector ShotFromDirection, class AController * InstigatedBy, AActor * DamageCauser, const FHitResult & HitInfo);*/

	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdateHealth(float HealthChange);

	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdateEnergy(float EnergyChange);

	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdateArmor(float ArmorChange);

	UFUNCTION(BlueprintCallable, Category = "Power")
		float UpdateDamage(float DamageChange);

	FTimerHandle TimerHandle_Task;

protected:

	void Balas(UWorld* const World);

	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

private:

	UPROPERTY(EditAnywhere, Category = BulletDecals)
		UMaterialInterface* BulletHoleDecal;

	void CreateBulletHole(FHitResult* Object, UWorld* const World);

public:

	FRotator GetViewRotation() const override;

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

