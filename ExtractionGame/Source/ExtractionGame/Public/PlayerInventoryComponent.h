

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
	virtual void OnRep_InventoryItems() override;
	virtual void BeginPlay() override;
	void SafeBeginPlay();
	
private:
	UPROPERTY()
	AExtractionGameCharacter* Character;

	virtual void AddItem(UItem* Item, int StackSize, bool bClientSimulation, int SlotID) override;
};
