// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "War_ArmoredProjectile.h"
#include "War_ArmoredCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine.h"

AWar_ArmoredProjectile::AWar_ArmoredProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(0.5f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AWar_ArmoredProjectile::OnHit);		// set up a notification for when this component hits something blocking
	SetRootComponent(CollisionComp);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 50000;
	ProjectileMovement->MaxSpeed = 0;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = Rebote;
	ProjectileMovement->Bounciness = 0.6f;
	ProjectileMovement->ProjectileGravityScale = 1.5f;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	ProjectileParticles = CreateDefaultSubobject<UParticleSystemComponent>(FName("ProjectileParticles"));
	ProjectileParticles->SetupAttachment(RootComponent);
}

void AWar_ArmoredProjectile::BeginPlay()
{
	Super::BeginPlay();
	Damage = 6.5f;
	Robo = 80.f;
	Reduccion = -50.f;
	Seguimiento = 10.f;

	ControlRebote = true;
	Rebote = true;

	MassBullet = 2.106f;
	BulletVel = 85.344f;
	EnergyDamage = 0; 
	RealBulletDamage = 0;
}

float AWar_ArmoredProjectile::CalculateDamage(float EnergyPlayer)
{
	float MassXBulletVell;
	float EnergyRef;
	float ForceBullet;
	float EnergyForce;
	float TotalDamage;

	float En = 0.356f;

	EnergyRef = EnergyPlayer / 65;
	EnergyForce = (EnergyRef * En);

	BulletVel = (BulletVel * BulletVel);
	MassBullet /= 2;

	MassXBulletVell = (MassBullet * BulletVel);
	ForceBullet = (MassXBulletVell / 1000);

	TotalDamage = (ForceBullet + (EnergyForce * 6));

	RealBulletDamage = TotalDamage;


	return TotalDamage;
}

void AWar_ArmoredProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherComp != NULL) && (OtherComp->IsSimulatingPhysics()) && (HitComp != NULL))
	{
		//OtherComp->AddImpulseAtLocation(GetVelocity() * 90.0f, GetActorLocation());
		//Destroy();
	}

	if (ControlRebote == false)
	{
		//Destroy();
	}
}

void AWar_ArmoredProjectile::RoboDeEnergia(float RoEnergy)
{
	RoEnergy = Robo;
}

void AWar_ArmoredProjectile::ReduccionDeEnergia(float ReEnergy)
{
	ReEnergy = Reduccion;
}

void AWar_ArmoredProjectile::VectorDeSeguimiento(AActor * VecEnemigo)
{
	FollowEnemigo = VecEnemigo->GetActorLocation();
}

void AWar_ArmoredProjectile::SeguirEnemigo(AActor * Enemigo)
{
	FollowEnemigo.X += Seguimiento;
	Enemigo->SetActorLocation(FollowEnemigo);
}