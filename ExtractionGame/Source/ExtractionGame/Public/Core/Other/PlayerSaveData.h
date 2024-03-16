

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
	
	UPROPERTY(BlueprintReadWrite)
	uint8 bHasSeenIntroCutscene;
	
};
