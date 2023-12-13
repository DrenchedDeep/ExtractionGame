

#pragma once

#include "CoreMinimal.h"
#include "Core/Other/ItemDatabase.h"
#include "Core/Other/PlayerSaveData.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerStashManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API APlayerStashManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<UItem*> StartingItems;

	bool bRanFirstTimeCheck;
	
public:	
	APlayerStashManager();
	virtual void BeginPlay() override;

	bool bInventoryLoaded;
	bool bInventorySaved;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStashInventoryComponent* PlayerInventory;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStashInventoryComponent* StashInventory;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UPlayerSaveData> PlayerSavedDataSubclass;

	UPROPERTY(EditDefaultsOnly)
	UItemDatabase* ItemDatabase;
	
	UFUNCTION(BlueprintCallable)
	void SaveInventory();
	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	UFUNCTION()
	void OnReadInventory(FString FileName, TArray<uint8> DataRef);
	UFUNCTION()
	void OnSavedInventory(bool bWasSuccess);
	UFUNCTION()
	void OnFilesRead(bool bWasSuccess);
};


