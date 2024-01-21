#include "UI/Widgets/GemSlot.h"
#include "Components/ItemObject.h"
#include "UI/Widgets/InventoryWidget.h"


void UGemSlot::InitNewGem(UItemObject* ItemObj)
{
	if(ItemObj)
	{
		SLotIconImage->SetVisibility(ESlateVisibility::Visible);
		SLotIconImage->SetBrushFromMaterial(ItemObj->Icon);
	}
	ItemObject = ItemObj;
}

void UGemSlot::ResetGemSlot()
{
	SLotIconImage->SetVisibility(ESlateVisibility::Hidden);
	ItemObject = nullptr;
}
