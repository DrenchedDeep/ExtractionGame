

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeyserSpawnpoint.generated.h"

UCLASS()
class EXTRACTIONGAME_API AGeyserSpawnpoint : public AActor
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_GeyserSpawned)
	bool bGeyserActivated;

	void ActivateGeyser();
	void DeactivateGeyser();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGeyserStatusUpdated();
	
protected:
	AGeyserSpawnpoint();

	UFUNCTION()
	void OnRep_GeyserSpawned();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
