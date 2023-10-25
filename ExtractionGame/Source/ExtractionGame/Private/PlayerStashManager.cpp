


#include "PlayerStashManager.h"

#include "MainMenuHUD.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "ExtractionGame/ExtractionGameInstance.h"

APlayerStashManager::APlayerStashManager()
{
	PlayerInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("PlayerInventory"));
	StashInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("StashInventory"));
}

void APlayerStashManager::BeginPlay()
{
	Super::BeginPlay();

	if(const AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		UStashInventoryWidget* InventoryWidget = MainMenuHUD->MainMenuWidget->StashInventoryWidget;

		PlayerInventory->Init(InventoryWidget, 20, true);
		StashInventory->Init(InventoryWidget, 40, false);
	}
}

void APlayerStashManager::SaveInventory()
{
	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		const TArray<FName> PartyMembers;
		GameInstance->BuildPlayerSessionData(PlayerInventory->StashItems, PartyMembers);
	}
}
