// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerCamManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class EXTRACTIONGAME_API APlayerCamManager : public APlayerCameraManager
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = .15f;

	float CrouchBlendTime;

public:
	APlayerCamManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaSeconds) override;
};
