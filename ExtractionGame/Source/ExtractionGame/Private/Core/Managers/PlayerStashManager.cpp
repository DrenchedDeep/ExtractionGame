#include "Core/Managers/PlayerStashManager.h"
#include "UI/MainMenuHUD.h"
#include "Core/Other/PlayerSaveData.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/ExtractionGame/ExtractionGameInstance.h"

APlayerStashManager::APlayerStashManager()
{
	PlayerInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("PlayerInventory"));
	StashInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("StashInventory"));
}

void APlayerStashManager::BeginPlay()
{
	//check if first time launch by getting files check if there is a playersavedata
	Super::BeginPlay();
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	
	GameInstance->GetFileCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnReadInventory);
	GameInstance->UserReadCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnFilesRead);
	GameInstance->UserWriteCompleteDelegate.AddDynamic(this, &APlayerStashManager::OnSavedInventory);

	if(const AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		UStashInventoryWidget* InventoryWidget = MainMenuHUD->MainMenuWidget->StashInventoryWidget;

		PlayerInventory->Init(InventoryWidget, 20, true);
		StashInventory->Init(InventoryWidget, 60, false);
	}

	StashInventory->InitStartingItems();

	
	LoadInventory();
}

void APlayerStashManager::SaveInventory()
{
	//disable for now
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	
	const TArray<FName> PartyMembers;
	GameInstance->BuildPlayerSessionData(PlayerInventory->StashItems, PartyMembers);

	bool bDisable = true;

	if(bDisable)
	{
		return;
	}
	const FString FileName = TEXT("PlayerSaveData");

	bool bHasSavedSlot = UGameplayStatics::DoesSaveGameExist(FileName, 0);

	if(bHasSavedSlot)
	{
		UGameplayStatics::DeleteGameInSlot(FileName, 0);
	}

	UPlayerSaveData* PlayerSavedData = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(PlayerSavedDataSubclass));

	UE_LOG(LogTemp, Warning, TEXT("Stash Length: %i"), StashInventory->StashItems.Num());
	UE_LOG(LogTemp, Warning, TEXT("Player Length: %i"), PlayerInventory->StashItems.Num());
	for(int i = 0; i < StashInventory->StashItems.Num(); i++)
	{
		const FInventoryItem InventoryItem = StashInventory->StashItems[i];

		if(InventoryItem.ItemID == nullptr)
		{
			continue;
		}
		
		const uint32 Value = (static_cast<uint32>(InventoryItem.SlotID) << 16)
		| (static_cast<uint32>(InventoryItem.ItemID->ItemID) << 8) | InventoryItem.StackSize;

		FSavedInventoryItem SavedInventoryItem(Value);
		PlayerSavedData->StashItems.Add(SavedInventoryItem);
	}

	for(int i = 0; i < PlayerInventory->StashItems.Num(); i++)
	{
		const FInventoryItem InventoryItem = PlayerInventory->StashItems[i];

		if(InventoryItem.ItemID == nullptr)
		{
			continue;
		}
		
		const uint32 Value = (static_cast<uint32>(InventoryItem.SlotID) << 16)
		| (static_cast<uint32>(InventoryItem.ItemID->ItemID) << 8) | InventoryItem.StackSize;

		FSavedInventoryItem SavedInventoryItem(Value);
		PlayerSavedData->PlayerItems.Add(SavedInventoryItem);
	}

	UGameplayStatics::SaveGameToSlot(PlayerSavedData, FileName, 0);

	TArray<uint8> SavedData = GameInstance->ConvertSavedFileToInt(PlayerSavedData);
	GameInstance->UpdatePlayerData(FileName, SavedData);
}

void APlayerStashManager::LoadInventory()
{
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	GameInstance->ReadPlayerData("PlayerSaveData");
}


void APlayerStashManager::OnReadInventory(FString FileName, TArray<uint8> DataRef)
{
	
	if(FileName != "PlayerSaveData")
	{
		return;
	}
	
	bool bHasSavedSlot = UGameplayStatics::DoesSaveGameExist(FileName, 0);

	if(bHasSavedSlot)
	{
		UGameplayStatics::DeleteGameInSlot(FileName, 0);
	}
	
	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());
	USaveGame* SaveGame = GameInstance->ConvertIntToSavedFile(DataRef);

	if(const UPlayerSaveData* PlayerSavedData = Cast<UPlayerSaveData>(SaveGame))
	{
		for(int i = 0; i < PlayerSavedData->StashItems.Num(); i++)
		{
			const uint8 SlotID = static_cast<uint8>((PlayerSavedData->StashItems[i].InventoryData >> 16) & 0xFF);
			const uint8 ItemID = static_cast<uint8>((PlayerSavedData->StashItems[i].InventoryData >> 8) & 0xFF);
			const uint8 StackSize = static_cast<uint8>(PlayerSavedData->StashItems[i].InventoryData & 0xFF);

			UItem* Item = ItemDatabase->FindItem(ItemID);
			StashInventory->AddItem(Item, StackSize, true, SlotID);
		}

		if(!GameInstance->bLoadedInventoryOnStart)
		{
			for(int i = 0; i < PlayerSavedData->PlayerItems.Num(); i++)
			{
				const uint8 SlotID = static_cast<uint8>((PlayerSavedData->PlayerItems[i].InventoryData >> 16) & 0xFF);
				const uint8 ItemID = static_cast<uint8>((PlayerSavedData->PlayerItems[i].InventoryData >> 8) & 0xFF);
				const uint8 StackSize = static_cast<uint8>(PlayerSavedData->PlayerItems[i].InventoryData & 0xFF);

				UItem* Item = ItemDatabase->FindItem(ItemID);
				PlayerInventory->AddItem(Item, StackSize, true, SlotID);
			}
			GameInstance->bLoadedInventoryOnStart = true;
		}
		const TArray<FName> PartyMembers;
		GameInstance->BuildPlayerSessionData(PlayerInventory->StashItems, PartyMembers);
		SaveInventory();
	}
}

void APlayerStashManager::OnSavedInventory(bool bWasSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("Saved Inventory"));
}

void APlayerStashManager::OnFilesRead(bool bWasSuccess)
{
		bRanFirstTimeCheck = true;
}



