

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSaveData.generated.h"

USTRUCT()
struct FSavedInventoryItem
{
	GENERATED_BODY()

	UPROPERTY()
	uint32 InventoryData;
	
	FSavedInventoryItem(uint32 InventoryData)
		: InventoryData(InventoryData)
	{
	}

	FSavedInventoryItem() = default;
};

UCLASS()
class EXTRACTIONGAME_API UPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
	UPROPERTY()
	bool bHasSeenIntroCutscene;
	UPROPERTY()
	bool bTutorialCrashLand; // After the player crash lands for the first time
	UPROPERTY()
	bool bTutorialInventory; // After the player equips a gem, and closes their inventory for the first time. Spam opening and closing a gem will cause the voiceline to be repeated.
	UPROPERTY()
	bool bTutorialExtraction; // After the player has enough essence to extract, and starts extracting for the first time.
	UPROPERTY()
	bool BTutorialDeath; // After the player has died for the very first time

	UPlayerSaveData();

public:
	UPROPERTY()
	TArray<FSavedInventoryItem> StashItems;
	UPROPERTY()
	TArray<FSavedInventoryItem> PlayerItems;
	UPROPERTY()
	TArray<FSavedInventoryItem> GemItems;
	

	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetSeenCutscene() const { return bHasSeenIntroCutscene;}
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetTutorialCrashLand() const { return bTutorialCrashLand;}
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetTutorialInventory() const { return bTutorialInventory;}
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetTutorialExtraction() const { return bTutorialExtraction;}
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetTutorialDeath() const { return BTutorialDeath;}
	
	
	UFUNCTION(BlueprintCallable) void SetSeenCutscene(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialCrashLand(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialInventory(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialExtraction(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialDeath(bool state);


	
};
