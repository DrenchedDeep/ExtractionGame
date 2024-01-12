

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "GameFramework/Actor.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ItemReplicationManager.generated.h"




/// This class is responsible for replicating PICKUPABLE items that exist in the world to clients
/// Since inventory system uses UObjects, we have to replicate them through an actor 


UCLASS()
class EXTRACTIONGAME_API AItemReplicationManager : public AActor
{
	GENERATED_BODY()
	
public:
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
