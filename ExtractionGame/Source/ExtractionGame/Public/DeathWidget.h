

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowScreen(const FString& PlayerName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRespawnTimerText(float RespawnTimer);
};
