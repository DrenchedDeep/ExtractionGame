

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TDMPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API ATDMPlayerState : public APlayerState
{
	GENERATED_BODY()


public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 Kills;
	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 Deaths;
	UPROPERTY(ReplicatedUsing=OnRep_TeamID, BlueprintReadOnly)
	uint8 TeamID;


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_TeamID();
};
