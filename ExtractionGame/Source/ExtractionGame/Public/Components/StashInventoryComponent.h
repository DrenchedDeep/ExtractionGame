

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "UI/Widgets/StashInventoryWidget.h"
#include "StashInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UStashInventoryComponent : public UInventoryComp
{
	GENERATED_BODY()

public:
	virtual void AddItem(int32 Index, UItemObject* Item) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
