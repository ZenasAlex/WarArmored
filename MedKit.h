// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "War_ArmoredCharacter.h"
#include "MedKit.generated.h"

UCLASS()
class WAR_ARMORED_API AMedKit : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMedKit();

	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	/*UFUNCTION()
		AWar_ArmoredCharacter* MyCharacter;*/

};
