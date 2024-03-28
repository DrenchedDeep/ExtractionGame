

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeyserSpawnpoint.generated.h"

UENUM(BlueprintType)
enum EGeyserStates
{
	Smoking,
	Active,
	Deactive
};

UCLASS()
class EXTRACTIONGAME_API AGeyserSpawnpoint : public AActor
{
	GENERATED_BODY()
	
	
public:
	void SetGeyserState(EGeyserStates NewState);
	void ResetGeyser();

	void InitGeyser(float smokeTime, float activeTime);

	float SmokeTime;
	float ActiveTime;

	UFUNCTION(BlueprintPure)
	TEnumAsByte<EGeyserStates> GetGeyserState() const { return State; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_GeyserState();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGeyserStatusUpdated();

private:
	UPROPERTY(ReplicatedUsing=OnRep_GeyserState)
	TEnumAsByte<EGeyserStates> State;

	FTimerHandle GeyserTimer;
	int32 CurrentGeyserTime;

	UFUNCTION()
	void GeyserTick();
};
