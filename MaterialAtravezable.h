// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialAtravezable.generated.h"

UCLASS()
class WAR_ARMORED_API AMaterialAtravezable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMaterialAtravezable();

	UPROPERTY(EditAnywhere)
		float MaterialResistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MaterialMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
