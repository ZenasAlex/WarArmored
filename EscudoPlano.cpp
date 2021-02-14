// Fill out your copyright notice in the Description page of Project Settings.


#include "EscudoPlano.h"

// Sets default values
AEscudoPlano::AEscudoPlano()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DebilitacionEP = 2;
	ResistenciaEP = 0;
	FullResistenciaEP = 100;

	MaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaterialMesh"));
}

float AEscudoPlano::DurabilidadEP(float Resistencia)
{
	ResistenciaEP = Resistencia;

	ResistenciaEP = ResistenciaEP / FullResistenciaEP;

	return ResistenciaEP;
}

// Called when the game starts or when spawned
void AEscudoPlano::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEscudoPlano::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

