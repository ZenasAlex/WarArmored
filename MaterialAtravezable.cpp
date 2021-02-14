// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialAtravezable.h"

// Sets default values
AMaterialAtravezable::AMaterialAtravezable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaterialResistance = 2;

	MaterialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MaterialMesh"));
}

// Called when the game starts or when spawned
void AMaterialAtravezable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMaterialAtravezable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

