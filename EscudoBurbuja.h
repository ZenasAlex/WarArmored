// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "War_Armored.h"
#include "GameFramework/Actor.h"
#include "EscudoBurbuja.generated.h"

UCLASS()
class WAR_ARMORED_API AEscudoBurbuja : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEscudoBurbuja();

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float DebilitacionEB;

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float ResistenciaEB;

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float FullResistenciaEB;

	UFUNCTION(BlueprintCallable, Category = "Escudo")
		float DurabilidadEB(float Resistencia);

private:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MaterialMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
