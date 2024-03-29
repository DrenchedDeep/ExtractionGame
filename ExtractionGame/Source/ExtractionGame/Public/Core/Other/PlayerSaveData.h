

#pragma once

#include "CoreMinimal.h"
#include "Components/GemController.h"
#include "Components/InventoryComp.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSaveData.generated.h"

USTRUCT(BlueprintType)
struct FSavedInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 Index;
	UPROPERTY(BlueprintReadOnly)
	FAddItemInfo ItemInfo;
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
	UPROPERTY()
	bool bLoadInventoryStartingItems;
	UPROPERTY()
	float ExtractedEssence;
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
	UFUNCTION(BlueprintCallable, BlueprintPure) bool GetLoadInventory() const { return bLoadInventoryStartingItems;}
	UFUNCTION(BlueprintCallable, BlueprintPure) float GetExtractedEssence() const { return ExtractedEssence;}

	UFUNCTION(BlueprintCallable, BlueprintPure) TArray<FSavedInventoryItem> GetStashInventory() const { return StashItems; }
	UFUNCTION(BlueprintCallable, BlueprintPure) TArray<FSavedInventoryItem> GetPlayerInventory() const { return PlayerItems; }
	UFUNCTION(BlueprintCallable, BlueprintPure) TArray<FSavedInventoryItem> GetGemInventory() const { return GemItems; }

	
	UFUNCTION(BlueprintCallable) void SetSeenCutscene(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialCrashLand(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialInventory(bool state);
	UFUNCTION(BlueprintCallable) void SetExtractedEssence(float InExtractedEssence);
	UFUNCTION(BlueprintCallable) void SetTutorialExtraction(bool state);
	UFUNCTION(BlueprintCallable) void SetTutorialDeath(bool state);
	UFUNCTION(BlueprintCallable) void SetLoadInventory(bool state);
	UFUNCTION(BlueprintCallable) void SetStashInventory(TMap<int32, FAddItemInfo> InInventory);
	UFUNCTION(BlueprintCallable) void SetPlayerInventory(TMap<int32, FAddItemInfo> InInventory);
	UFUNCTION(BlueprintCallable) void SetGemInventory(TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> InInventory);



	
};
