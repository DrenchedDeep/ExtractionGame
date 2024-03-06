


#include "ShopManager.h"

AShopManager::AShopManager()
{
	ShopInventory = CreateDefaultSubobject<UStashInventoryComponent>(TEXT("ShopInventory"));
}

