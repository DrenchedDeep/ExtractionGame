

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void OnRep_InventoryItems() override;

private:
	UPROPERTY()
	AExtractionGameCharacter* Character;

	virtual void Server_AddItem_Implementation(UItem* Item, int StackSize, int InventoryIndex, int SlotID) override;

};
