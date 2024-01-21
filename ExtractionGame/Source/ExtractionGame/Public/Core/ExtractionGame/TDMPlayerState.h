

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
	UPROPERTY(ReplicatedUsing=OnRep_Kills, BlueprintReadOnly, VisibleAnywhere)
	uint8 Kills;
	UPROPERTY(ReplicatedUsing=OnRep_Deaths, BlueprintReadOnly, VisibleAnywhere)
	uint8 Deaths;
	UPROPERTY(Replicated, BlueprintReadOnly, VisibleAnywhere)
	uint8 TeamID;


	UFUNCTION()
	void OnRep_Kills();

	UFUNCTION()
	void OnRep_Deaths();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
