

#pragma once

#include "CoreMinimal.h"
#include "InventoryComp.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

enum EBodyPart : uint8;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FGemItem
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	UItemObject* Item;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EBodyPart> BodyPart;
};


UCLASS()
class EXTRACTIONGAME_API UPlayerInventoryComponent : public UInventoryComp
{
	GENERATED_BODY()

public:
	void InitStartingItems();
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddGem(class UItemObject* Item, EBodyPart BodyPart);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddGemRaw(FAddItemInfo Item, EBodyPart BodyPart);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RemoveGem(UItemObject* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UItemObject*> GetItemsIncludingGems();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FGemItem> GetGemItems() const { return GemItems; }
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_GemItems();
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_GemItems)
	 TArray<FGemItem> GemItems;
	
};
