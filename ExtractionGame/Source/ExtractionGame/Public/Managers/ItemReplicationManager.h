

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "GameFramework/Actor.h"
#include "Items/ItemActor.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ItemReplicationManager.generated.h"



/// This class is responsible for replicating PICKUPABLE items that exist in the world to clients
/// Since inventory system uses UObjects, we have to replicate them through an actor 


///replicate list of items that hold state, seperate visuals from state!!
///give each item 
///when player picks up item, get the index and tell all clients to destroy that item from server
///

UCLASS()
class EXTRACTIONGAME_API UItemReplicationManager : public UActorComponent
{
	GENERATED_BODY()



protected:
	virtual void BeginPlay() override;
};
