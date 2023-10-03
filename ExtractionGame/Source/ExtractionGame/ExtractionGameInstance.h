

#pragma once

#include "CoreMinimal.h"
#include "MainMenuGameState.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "ExtractionGameInstance.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCompleted, bool, bWasSuccess);


UCLASS()
class EXTRACTIONGAME_API UExtractionGameInstance : public UGameInstance
{
	GENERATED_BODY()

	IOnlineSubsystem* OnlineSubSystem;
	IOnlineIdentityPtr UserIdentity;
	IOnlineSessionPtr Session;
	
	FNamedOnlineSession* CurrentSession;

	bool bIsPartyHost;
	FTimerHandle DelayJoinSession;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnLoginCompleted OnLoginComplete;
	
	UFUNCTION(BlueprintCallable)
	void LoginEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLoggedIn();
	
	void CreateSession(int32 PlayerCount);
	void JoinSession(const FClientConnectionInfo ConnectionInfo);

	UFUNCTION(BlueprintCallable) void JoinSession();
	UFUNCTION(BlueprintCallable) bool CreateLobby();
	UFUNCTION(BlueprintCallable) void DestroySession();

	void OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	void SetupOnlineSubsystem();

	void DelaySessionJoin(const FClientConnectionInfo ConnectionInfo);
	
	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search);
	void HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);
	
	
	virtual void Shutdown() override;
};




