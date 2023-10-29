#include "StashInventoryWidget.h"

#include "PlayerStashManager.h"
#include "StashInventoryComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"


void UStashInventoryWidget::InitInventoryWidget(UStashInventoryComponent* InventoryComponent, int32 PlayerInventorySize,
                                                int32 StashInventorySize)
{
	StashGridPanel->Init(InventoryComponent, StashInventorySize);
	PlayerInventoryGridPanel->Init(InventoryComponent, PlayerInventorySize);
}


void UStashInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerStashManager = Cast<APlayerStashManager>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStashManager::StaticClass()));
	ToggleInventory(true);
	ToggleSavingScreen(false);
	ToggleLoadingScreen(false);
}

void UStashInventoryWidget::EnableUI()
{
}

void UStashInventoryWidget::DisableUI()
{
}

void UStashInventoryWidget::OnInventorySaved()
{
}

void UStashInventoryWidget::OnInventoryLoaded()
{
}

