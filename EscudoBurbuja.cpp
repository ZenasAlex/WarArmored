// Fill out your copyright notice in the Description page of Project Settings.


#include "EscudoBurbuja.h"

// Sets default values
AEscudoBurbuja::AEscudoBurbuja()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DebilitacionEB = 2;
	ResistenciaEB = 0;
	FullResistenciaEB = 100;

	MaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaterialMesh"));
}

// Called when the game starts or when spawned
void AEscudoBurbuja::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEscudoBurbuja::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEscudoBurbuja::DurabilidadEB(float Resistencia)
{
	ResistenciaEB = Resistencia;

	ResistenciaEB = ResistenciaEB / FullResistenciaEB;

	return ResistenciaEB;
}

