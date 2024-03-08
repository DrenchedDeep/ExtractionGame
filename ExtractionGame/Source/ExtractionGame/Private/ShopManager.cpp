


#include "ShopManager.h"

AShopManager::AShopManager()
{
	ShopInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("ShopInventory"));
	BuyInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("BuyInventory"));
	SellInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("SellInventory"));
}

