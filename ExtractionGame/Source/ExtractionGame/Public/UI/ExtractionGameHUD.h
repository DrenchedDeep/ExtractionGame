

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/DeathWidget.h"
#include "GameHUD.h"
#include "Core/ExtractionGame/ExtractionGameGameMode.h"
#include "UI/Widgets/InventoryWidget.h"
#include "UI/Widgets/PlayerBarDataWidget.h"
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
	UPROPERTY(BlueprintReadWrite)
	UPlayerBarDataWidget* PlayerUIData;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPlayerBarDataWidget* GetPlayerBarWidget() const;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetSubclass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDeathWidget> DeathWidgetSubclass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerBarDataWidget> PlayerUIDataSubclass;
	
	UInventoryWidget* CreateInventoryWidget();
	void CreatePlayerBarDataWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerKilled(const FString& PlayerName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatchTimerText(int32 MatchTimer);
	UFUNCTION(BlueprintImplementableEvent)
	void EndGame(const FString& WinningTeam);

	UFUNCTION(BlueprintImplementableEvent)
	void OnKill(const FString& VictimName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 Kills, int32 Deaths);
	
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateExtractionTimerWidget(float ExtractionTimer);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStateChanged(EGameModeState NewState);
	
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void ToggleRespawnWidget(bool bEnable);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCompass(bool bEnable);
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleStats(bool bEnable);
	
	UPROPERTY(BlueprintReadWrite)
	UUserWidget* ExtractionWidget;
private:
	
	virtual void BeginPlay() override;
};
