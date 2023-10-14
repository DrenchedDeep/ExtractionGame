

#pragma once

#include "CoreMinimal.h"
#include "SlotWidget.h"
#include "GemSlot.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UGemSlot : public USlotWidget
{
	GENERATED_BODY()

public:
	virtual void TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID) override;
	virtual void PredictVisuals(UItem* Item, int Stack) override;
};
