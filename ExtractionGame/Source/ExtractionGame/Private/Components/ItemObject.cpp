


#include "Components/ItemObject.h"

#include "Net/UnrealNetwork.h"

void UItemObject::Rotate()
{
	bRotated = !bRotated;
}

void UItemObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemObject, Dimensions);
	DOREPLIFETIME(UItemObject, Icon);
	DOREPLIFETIME(UItemObject, IconRotated);
	DOREPLIFETIME(UItemObject, ItemName);
	DOREPLIFETIME(UItemObject, ItemType);
	DOREPLIFETIME(UItemObject, Rarity);
	DOREPLIFETIME(UItemObject, Description);
	DOREPLIFETIME(UItemObject, GemType);
	DOREPLIFETIME(UItemObject, DefaultPolish);
}
