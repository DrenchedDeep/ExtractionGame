

#pragma once

#include "CoreMinimal.h"
#include "Goober.h"
#include "GooberSpawnCluster.h"
#include "GooberTask.h"
#include "GameFramework/Actor.h"
#include "GooberManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API AGooberManager : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGoober> GooberSubclass;
	UPROPERTY(EditAnywhere)
	float RespawnRate = 20.f;
	UPROPERTY(EditAnywhere)
	bool bOnlyAllowSameClusterInStreamOnce = true;
	
	UFUNCTION(BlueprintPure)
	TArray<AGoober*> GetAllGoobers();

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_PlayExplosion(FVector Location);
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_PickupGem(AActor* GemActor, AGoober* Goober);
	void AddToRespawnStream(AGooberSpawnCluster* Cluster);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TickGooberRespawnHandle();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayExplosionEffect(FVector Location);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPickupGem(AActor* GemActor, AGoober* Goober);
private:
	UPROPERTY()
	TArray<AGooberSpawnCluster*> SpawnClusters;
	UPROPERTY()
	TArray<AGooberSpawnCluster*> SpawnBuffer;
	
	FTimerHandle GooberRespawnHandle;
	
};
