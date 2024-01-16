#include "UI/Widgets/StashInventoryWidget.h"

#include "Components/StashInventoryComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


void UStashInventoryWidget::InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize,
                                                int32 StashInventorySize)
{
	//StashGridPanel->Init(InventoryComponent, StashInventorySize);
//	PlayerInventoryGridPanel->Init(InventoryComponent, PlayerInventorySize);
}


void UStashInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStashInventoryWidget::EnableUI(bool bEnableUI)
{
	Super::EnableUI(bEnableUI);
}

void UStashInventoryWidget::DisableUI(bool bDisableUI)
{
	Super::DisableUI(bDisableUI);
}


void UStashInventoryWidget::OnInventorySaved()
{
}

void UStashInventoryWidget::OnInventoryLoaded()
{
}

