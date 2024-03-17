

#pragma once

#include "CoreMinimal.h"
#include "GeyserSpawnpoint.h"
#include "GameFramework/Actor.h"
#include "HamiltonBuildOutline.generated.h"

UCLASS()
class EXTRACTIONGAME_API AHamiltonBuildOutline : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	bool IsLocationBuildable() const;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMaterials();
	
protected:
	virtual void BeginPlay() override;

private:
	float MinDistFromGeyser = 3500.f;
	
	UPROPERTY()
	TArray<AGeyserSpawnpoint*> GeyserSpawnpoints;
};
