// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

	float CurrentHealth;
	
	float Health;
	
public:	
	UPlayerHealthComponent();
	

	float GetCurrentHealth() const { return CurrentHealth; }
};
