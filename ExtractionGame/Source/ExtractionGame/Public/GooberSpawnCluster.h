

#pragma once

#include "CoreMinimal.h"
#include "AI/Goober.h"
#include "GameFramework/Actor.h"
#include "GooberSpawnCluster.generated.h"

class AGooberManager;
UCLASS()
class EXTRACTIONGAME_API AGooberSpawnCluster : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	int32 MaxGoobersToSpawn;
	UPROPERTY(EditAnywhere)
	bool bMakeSureGooberCountIsAtMax = true;
	UPROPERTY(EditAnywhere)
	float SpawnRadius = 300;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> GooberSpawnPoints;
	
public:
	void SpawnGoobers(AGooberManager* InGooberManager);
	void UnRegisterGoober(AGoober* Goober);
	void SpawnGoober();

	
	TArray<AGoober*> GetAllGoobers() const { return AllGoobers; }

private:
	UPROPERTY()
	TArray<AGoober*> AllGoobers;
	UPROPERTY()
	AGooberManager* GooberManager;
	
};
