

#pragma once

#include "CoreMinimal.h"
#include "DeathWidget.h"
#include "GameHUD.h"
#include "InventoryWidget.h"
#include "GameFramework/HUD.h"
#include "ExtractionGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGameHUD : public AGameHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;
	UPROPERTY(BlueprintReadWrite)
	UDeathWidget* DeathWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetSubclass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDeathWidget> DeathWidgetSubclass;
	
	UInventoryWidget* CreateInventoryWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerKilled(const FString& PlayerName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatchTimerText(int32 MatchTimer);
	UFUNCTION(BlueprintImplementableEvent)
	void EndGame(const FString& WinningTeam);
private:
	virtual void BeginPlay() override;
};
