// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExtractionGamePlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ReturnToLobby();
	
protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;
	
	virtual void BeginPlay() override;
	virtual void ClientWasKicked_Implementation(const FText& KickReason) override;
};
