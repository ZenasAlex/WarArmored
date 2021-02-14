// Fill out your copyright notice in the Description page of Project Settings.


#include "Dummy.h"
#include "War_ArmoredCharacter.h"
#include "Engine.h"

// Sets default values
ADummy::ADummy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 1000;
	Energy = 1000;
	Reduction = 2.5;

	HealthText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthText"));
	HealthText->SetWorldSize(50.f);
	HealthText->SetTextRenderColor(FColor::Red);
	HealthText->SetHorizontalAlignment(EHTA_Center);

	DummyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMesh"));

	AcDummy = Cast<ADummy>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called when the game starts or when spawned
void ADummy::BeginPlay()
{
	Super::BeginPlay();

	HealthText->SetText(FString::Printf(TEXT("Health: %d %"), Health));
}

// Called every frame
void ADummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeTextFacePlayer();
}

void ADummy::DamageDummy(int32 Damage)
{
	Health -= Damage;

	if (Health <= 0)
	{
		Destroy();
	}
	else
	{
		HealthText->SetText(FString::Printf(TEXT("Health: %d %"), Health));
	}
}

void ADummy::StealDummy(int32 Steal)
{
	Energy -= Steal;

	if (Health <= 0)
	{
		Destroy();
	}
	else
	{
	}
}

void ADummy::ReductionDummy(int32 Reduccion)
{
	Energy /= (-Reduccion);

	if (Health <= 0)
	{
		Destroy();
	}
	else
	{
	}
}

void ADummy::MakeTextFacePlayer()
{
	AWar_ArmoredCharacter* Character = Cast<AWar_ArmoredCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	FRotator NewRotation = Character->GetActorRotation();
	NewRotation.Yaw += 180.f;

	HealthText->SetRelativeRotation(NewRotation);
}