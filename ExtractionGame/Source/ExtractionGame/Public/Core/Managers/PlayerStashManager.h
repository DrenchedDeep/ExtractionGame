

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerInventoryComponent.h"
#include "Core/Other/ItemDatabase.h"
#include "Core/Other/PlayerSaveData.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerStashManager.generated.h"

USTRUCT(BlueprintType)
struct FAddItemStashManager
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Polish;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Item;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Count;
};

UCLASS()
class EXTRACTIONGAME_API APlayerStashManager : public AActor
{
	GENERATED_BODY()


	bool bRanFirstTimeCheck;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemObject> ItemObjectSubclass;
	
public:	
	APlayerStashManager();
	virtual void BeginPlay() override;

	bool bInventoryLoaded;
	bool bInventorySaved;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAddItemStashManager> StartingItems;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAddItemStashManager> GemStartingItems;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* PlayerInventory;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* StashInventory;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UPlayerSaveData> PlayerSavedDataSubclass;
	
	
	UFUNCTION(BlueprintCallable)
	void SaveInventory();

	UFUNCTION(BlueprintImplementableEvent)
	void OnSave();
	UFUNCTION(BlueprintImplementableEvent)
	void OnGemInventoryChanged();
	UFUNCTION(BlueprintImplementableEvent)
	void InitStartingItems();
	UFUNCTION(BlueprintCallable)
	void AddGem(UItemObject* Gem, EBodyPart BodyPart);
	UFUNCTION(BlueprintCallable)
	void RemoveGem(EBodyPart BodyPart);
	
	TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GetGemInventory();

	UFUNCTION(BlueprintCallable)
	TArray<FGemItem> GetGemInventoryStruct();
	UFUNCTION(BlueprintImplementableEvent)
	void InitGemUI();

	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetLeftArmAbilityInfo();
	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetRightArmAbilityInfo();

private:
	TMap<TEnumAsByte<EBodyPart>, UItemObject*> GemInventory;
};


