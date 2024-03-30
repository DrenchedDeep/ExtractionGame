

#pragma once

#include "CoreMinimal.h"
#include "GeyserController.h"
#include "GeyserSpawnpoint.h"
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
	UPROPERTY(EditAnywhere)
	float GooberGeyserRange = 4000.f;
	UFUNCTION(BlueprintPure)
	TArray<AGoober*> GetAllGoobers();
	static TWeakObjectPtr<AGooberManager> GooberInstance;

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_PlayExplosion(FVector Location);
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_PickupGem(AActor* GemActor, AGoober* Goober);
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_Attack(AGoober* Goober);
	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void Multicast_ResetGem(AActor* Gem);
	
	void AddToRespawnStream(AGooberSpawnCluster* Cluster);
	void OnGeyserOpened(AGeyserSpawnpoint* Geyser);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TickGooberRespawnHandle();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayExplosionEffect(FVector Location);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayPickupGem(AActor* GemActor, AGoober* Goober);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAttackAnim(AGoober* Goober);
	UFUNCTION(BlueprintImplementableEvent)
	void ResetGemSize(AActor* Gem);

private:
	UPROPERTY()
	TArray<AGooberSpawnCluster*> SpawnClusters;
	UPROPERTY()
	TArray<AGooberSpawnCluster*> SpawnBuffer;
	
	FTimerHandle GooberRespawnHandle;
	
};
