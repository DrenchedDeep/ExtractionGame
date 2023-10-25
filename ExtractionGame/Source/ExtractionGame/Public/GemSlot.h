

#pragma once

#include "CoreMinimal.h"
#include "SlotWidget.h"
#include "Components/GemController.h"
#include "GemSlot.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UGemSlot : public USlotWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* SlotBackgroundImage;
public:

	UPROPERTY(BlueprintReadOnly)
	AGem* Gem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EBodyPart> BodyPart;
	
	virtual void TransferSlots(UInventoryComponent* SourceInventoryComponent, int TargetSlotID) override;
	virtual void PredictVisuals(UItem* Item, int Stack) override;
	virtual void ReconcileVisuals(FInventoryItem& Item) override;
	virtual void Reset() override;
};	