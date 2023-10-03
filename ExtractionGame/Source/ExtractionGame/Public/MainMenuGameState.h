

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "OnlineSessionSettings.h"
#include "MainMenuGameState.generated.h"

USTRUCT()
struct FClientConnectionInfo
{
	GENERATED_BODY()

	bool bIsValid;
	FName SessionName;
	FOnlineSessionSearchResult SearchResult;

	FClientConnectionInfo(bool bIsValid, const FName& SessionName, const FOnlineSessionSearchResult& SearchResult)
		: bIsValid(bIsValid),
		  SessionName(SessionName),
		  SearchResult(SearchResult)
	{
	}

	FClientConnectionInfo() = default;
};

UCLASS()
class EXTRACTIONGAME_API AMainMenuGameState : public AGameStateBase
{
	GENERATED_BODY()

	FTimerHandle FindSessionTimer;
	
public:
	AMainMenuGameState(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	FClientConnectionInfo ConnectionInfo;
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SendConnectionInfo(FClientConnectionInfo ConnectInfo);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_JoinSession(FClientConnectionInfo ConnectInfo);

	void FindSession();
};
