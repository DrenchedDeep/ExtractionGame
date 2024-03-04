

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineSessionSettings.h"
#include "Core/Managers/PartyManager.h"
#include "Core/Managers/PlayerStashManager.h"
#include "MainMenuGameState.generated.h"

USTRUCT()
struct FClientConnectionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsValid;
	UPROPERTY()
	FName SessionName;

	FClientConnectionInfo(bool bIsValid, const FName& SessionName)
		: bIsValid(bIsValid),
		  SessionName(SessionName)
	{
	}

	FClientConnectionInfo() = default;
};

UCLASS()
class EXTRACTIONGAME_API AMainMenuGameState : public AGameStateBase
{
	GENERATED_BODY()

	bool bAllClientsLoadedSession = false;

public:
	AMainMenuGameState(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, BlueprintReadOnly)
	APartyManager* PartyManager;

	UPROPERTY()
	FClientConnectionInfo ConnectionInfo;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SendConnectionInfo(FClientConnectionInfo ConnectInfo);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_JoinSession(FClientConnectionInfo ConnectInfo);
	
	bool bIsConnectingToGameSession;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
