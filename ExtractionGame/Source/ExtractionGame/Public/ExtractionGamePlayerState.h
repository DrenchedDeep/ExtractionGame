

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ExtractionGamePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	float GetEssence() const { return ExtractedEssence; }
	UFUNCTION(BlueprintCallable)int32 GetKills() const { return Kills; }
	UFUNCTION(BlueprintCallable)int32 GetGooberKills() const { return GooberKills; }
	UFUNCTION(BlueprintCallable)int32 GetDeaths() const { return Deaths; }

	void AddEssence(int32 EssenceToAdd);
	void AddKill() { Kills++; }
	void AddDeath() { Deaths++; }
	void AddGooberKill() { GooberKills++; } 
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY(Replicated)
	int32 ExtractedEssence;
	UPROPERTY(Replicated)int32 Kills;
	UPROPERTY(Replicated)int32 GooberKills;
	UPROPERTY(Replicated)int32 Deaths;
};
