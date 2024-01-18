

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "GameFramework/Actor.h"
#include "Items/ItemActor.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ItemReplicationManager.generated.h"


USTRUCT()
struct FItemPickup 
{
	GENERATED_BODY()

	UPROPERTY()
	AItemActor* OwnerActor;

	UPROPERTY()
	bool bIsPickedUp;
};



/// This class is responsible for replicating PICKUPABLE items that exist in the world to clients
/// Since inventory system uses UObjects, we have to replicate them through an actor 


UCLASS()
class EXTRACTIONGAME_API UItemReplicationManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FItemPickup> ItemPickups;

	void Init();
	
	int32 RegisterNewItem(AItemActor* ItemActor);
	void UpdateItemState(int32 Index, bool bIsPickedUp);
	void UnregisterItem(int32 Index);

	UFUNCTION(Server, Reliable)
	void Server_UpdateItemState(int32 Index, bool bIsPickedUp);

	void SpawnItems();
	
protected:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateItem(AItemActor* ItemActor);
};
