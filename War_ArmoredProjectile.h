// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "War_ArmoredProjectile.generated.h"

UCLASS(config = Game)
class AWar_ArmoredProjectile : public AActor
{
	GENERATED_BODY()
public:
	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditDefaultsOnly, Category = Particles)
		UParticleSystemComponent* ProjectileParticles;

	virtual void BeginPlay();

public:
	AWar_ArmoredProjectile();

	UPROPERTY(EditAnywhere)
		AActor* FpsCharacter;

	UPROPERTY(EditAnywhere)
		FHitResult BulletHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rebote")
		bool Rebote;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rebote")
		bool ControlRebote;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robo")
		float Robo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reduccion")
		float Reduccion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Damage")
		float RealBulletDamage;

	UPROPERTY(Editanywhere, category = "Damage")
		float MassBullet;

	UPROPERTY(Editanywhere, category = "Damage")
		float BulletVel;

	UPROPERTY(Editanywhere, category = "Damage")
		float EnergyDamage;

	UPROPERTY(Editanywhere, category = "Damage")
		float Distancia;

	UFUNCTION(BlueprintCallable, category = "Damage")
		float CalculateDamage(float energyPlayer);

	float Damage;
	float Seguimiento;
	FVector FollowEnemigo;

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void RoboDeEnergia(float RoEnergy);
	void ReduccionDeEnergia(float ReEnergy);

	void VectorDeSeguimiento(AActor* VecEnemigo);
	void SeguirEnemigo(AActor* Enemigo);
};

