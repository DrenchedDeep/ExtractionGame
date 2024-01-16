

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


	bool bRanFirstTimeCheck;
	
public:	
	APlayerStashManager();
	virtual void BeginPlay() override;

	bool bInventoryLoaded;
	bool bInventorySaved;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> StartingItems;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* PlayerInventory;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* StashInventory;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UPlayerSaveData> PlayerSavedDataSubclass;

	UPROPERTY(EditDefaultsOnly)
	UItemDatabase* ItemDatabase;
	
	UFUNCTION(BlueprintCallable)
	void SaveInventory();

	UFUNCTION(BlueprintCallable)
	void AddGem(UItemObject* Gem, EBodyPart BodyPart);
	UFUNCTION(BlueprintCallable)
	void RemoveGem(EBodyPart BodyPart);
	
	TMap<int32,FAddItemInfo> GetPlayerInventory() const;
	TMap<int32, FAddItemInfo> GetStashInventory() const;
	TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GetGemInventory();


private:
	TMap<TEnumAsByte<EBodyPart>, UItemObject*> GemInventory;
};


