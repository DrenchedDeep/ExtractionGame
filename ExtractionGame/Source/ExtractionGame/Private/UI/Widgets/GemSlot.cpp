#include "UI/Widgets/GemSlot.h"
#include "Components/ItemObject.h"
#include "UI/Widgets/InventoryWidget.h"


void UGemSlot::InitNewGem(UItemObject* ItemObj)
{
	ItemObject = ItemObj;
	
	SLotIconImage->SetVisibility(ESlateVisibility::Visible);
	SLotIconImage->SetBrushFromMaterial(ItemObj->Icon);
}

void UGemSlot::ResetGemSlot()
{
	SLotIconImage->SetVisibility(ESlateVisibility::Hidden);
	ItemObject = nullptr;
}
