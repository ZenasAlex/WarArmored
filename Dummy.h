// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GameFramework/Actor.h"
#include "Dummy.generated.h"

UCLASS()
class WAR_ARMORED_API ADummy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADummy();

	void DamageDummy(int32 Damage);
	void StealDummy(int32 Steal);
	void ReductionDummy(int32 Reduccion);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	AActor* AcDummy;

	UTextRenderComponent* HealthText;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* DummyMesh;

	UPROPERTY(EditAnywhere, Category = "Movimiento")

		int32 Health;
	int32 Energy;
	int32 Reduction;

	void MakeTextFacePlayer();
};
