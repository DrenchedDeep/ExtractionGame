


#include "SlotWidget.h"
#include "Item.h"
#include "Components/TextBlock.h"

void USlotWidget::Init(int ID)
{
	SlotID = ID;
	SlotIconImage->SetVisibility(ESlateVisibility::Hidden);
	SlotStackText->SetVisibility(ESlateVisibility::Hidden);
}

void USlotWidget::InitItem(UItem* Item, int StackSize)
{
	bIsOccupied = true;
	GLog->Log(TEXT("InitItem"));
	SlotIconImage->SetVisibility(ESlateVisibility::Visible);
	SlotIconImage->SetBrushFromTexture(Item->ItemIcon);
	SlotStackText->SetText(FText::FromString(FString::FromInt(StackSize)));
	SlotStackText->SetVisibility(ESlateVisibility::Visible);
}
