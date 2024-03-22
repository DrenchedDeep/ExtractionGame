

#pragma once

#include "CoreMinimal.h"
#include "ExtractionGamePlayerState.h"
#include "GameFramework/Actor.h"
#include "EndGamePlauerStand.generated.h"

UCLASS()
class EXTRACTIONGAME_API AEndGamePlauerStand : public AActor
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	AExtractionGamePlayerState* GetStandOwner() const { return StandOwner; }
	
	void SetStandOwner(AExtractionGamePlayerState* NewOwner);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AEndGamePlauerStand();

	
	UFUNCTION()
	void OnRep_StandOwner();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStandOwnerChanged();
private:
	UPROPERTY(ReplicatedUsing=OnRep_StandOwner)
	AExtractionGamePlayerState* StandOwner;
};
