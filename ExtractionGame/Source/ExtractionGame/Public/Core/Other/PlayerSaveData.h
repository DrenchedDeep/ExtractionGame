

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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

public:
	UPROPERTY()
	TArray<FSavedInventoryItem> StashItems;
	UPROPERTY()
	TArray<FSavedInventoryItem> PlayerItems;
	UPROPERTY()
	TArray<FSavedInventoryItem> GemItems;
	
	UPROPERTY(BlueprintReadWrite) bool bHasSeenIntroCutscene;
	
	UPROPERTY(BlueprintReadWrite) bool bTutorialCrashLand; // After the player crash lands for the first time
	UPROPERTY(BlueprintReadWrite) bool bTutorialInventory; // After the player equips a gem, and closes their inventory for the first time. Spam opening and closing a gem will cause the voiceline to be repeated.
	UPROPERTY(BlueprintReadWrite) bool bTutorialExtraction; // After the player has enough essence to extract, and starts extracting for the first time.
	UPROPERTY(BlueprintReadWrite) bool BTutorialDeath; // After the player has died for the very first time

	

	
};
