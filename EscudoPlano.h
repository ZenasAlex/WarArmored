// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "War_Armored.h"
#include "GameFramework/Actor.h"
#include "EscudoPlano.generated.h"

UCLASS()
class WAR_ARMORED_API AEscudoPlano : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEscudoPlano();

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float DebilitacionEP;

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float ResistenciaEP;

	UPROPERTY(EditAnywhere, Category = "Escudo")
		float FullResistenciaEP;

	UFUNCTION(BlueprintCallable, Category = "Escudo")
		float DurabilidadEP(float Resistencia);

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
