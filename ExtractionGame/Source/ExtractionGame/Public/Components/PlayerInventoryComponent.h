

#pragma once

#include "CoreMinimal.h"
#include "InventoryComp.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UPlayerInventoryComponent : public UInventoryComp
{
	GENERATED_BODY()

public:
	void InitStartingItems();


	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AddGem(UItemObject* Item);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_RemoveGem(UItemObject* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UItemObject*> GetItemsIncludingGems();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	TArray<UItemObject*> GemItems;
	
};
