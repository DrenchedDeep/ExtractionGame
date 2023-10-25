

#pragma once

#include "CoreMinimal.h"
#include "GemSlot.h"
#include "Blueprint/UserWidget.h"
#include "StashWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init(UInventoryComponent* InventoryComponent, int32 Size, bool bInitGemSlots = false);
};