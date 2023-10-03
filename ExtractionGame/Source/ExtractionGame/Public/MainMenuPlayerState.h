

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MainMenuPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AMainMenuPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UPROPERTY(ReplicatedUsing=OnRep_PlayerReady)
	bool bPlayerReadyForMatch;

	UFUNCTION(Server, Reliable)
	void Server_SetUsername(const FString& Username);
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_ToggleReadyStatus(bool bReady);

	UFUNCTION()
	void OnRep_PlayerReady();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
