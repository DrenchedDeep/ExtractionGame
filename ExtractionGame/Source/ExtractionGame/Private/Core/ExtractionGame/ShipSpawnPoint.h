

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipSpawnPoint.generated.h"

UCLASS()
class AShipSpawnPoint : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditDefaultsOnly)
	float innerRing;
	UPROPERTY(EditDefaultsOnly)
	float outerRing;
	UPROPERTY(EditDefaultsOnly)
	float minHeight;
	UPROPERTY(EditDefaultsOnly)
	float maxHeight;
	UPROPERTY(EditDefaultsOnly)
	float additionalUpOffset;
	
public:	
	// Sets default values for this actor's properties
	AShipSpawnPoint();

	UFUNCTION(BlueprintCallable)
	void MoveToPosition(AActor* actor);
};
