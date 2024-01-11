

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "GameFramework/Actor.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ItemReplicationManager.generated.h"
/*/

USTRUCT()
struct FItemActorItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	TArray<uint8> Data;

	FItemActorItem();
	FItemActorItem(AActor* InOwnerActor, const TArray<uint8>& InData);

	void PostReplicatedChange(const struct FItemActorContainer& InArraySerializer);
};

USTRUCT()
struct FItemActorContainer : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FItemActorItem> Items;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FItemActorItem, FItemActorContainer>(Items, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FItemActorContainer> : public TStructOpsTypeTraitsBase2<FItemActorContainer>
{
	enum
	{
		WithNetDeltaSerializer = true,
	};
};
/*/



/// This class is responsible for replicating PICKUPABLE items that exist in the world to clients
/// Since inventory system uses UObjects, we have to replicate them through an actor 


UCLASS()
class EXTRACTIONGAME_API AItemReplicationManager : public AActor
{
	GENERATED_BODY()
	
public:
	/*/
	AItemReplicationManager();

	UPROPERTY(Replicated)
	FItemActorContainer ItemActors;

	UFUNCTION(BlueprintCallable)
	void RegisterActor(AActor* Actor, const TArray<uint8>& Data);

	UFUNCTION(BlueprintCallable)
	void UpdateActor(AActor* Actor, const TArray<uint8>& Data);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	/*/
};
