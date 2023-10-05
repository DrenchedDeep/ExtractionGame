#pragma once

#include "CoreMinimal.h"
#include "MainMenuGameState.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Core/AbilityHandlerSubSystem.h"
#include "ExtractionGameInstance.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCompleted, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartSessionComplete, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndSessionComplete, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroySessionComplete, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoinSessionComplete, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSessionComplete, bool, bWasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateLobbyComplete, bool, bWasSuccess);

UCLASS()
class EXTRACTIONGAME_API UExtractionGameInstance : public UGameInstance
{
	GENERATED_BODY()

	const int SESSION_PLAYERCOUNT = 4;
	
	IOnlineSubsystem* OnlineSubSystem;
	IOnlineIdentityPtr UserIdentity;
	IOnlineSessionPtr Session;
	FNamedOnlineSession* CurrentSession;
	
	UPROPERTY()
	UAbilityHandlerSubSystem* AbilityHandlerSubSystem;

public:
	UExtractionGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void SetupOnlineSubsystem();

	UFUNCTION(BlueprintCallable) void JoinSession();
	UFUNCTION(BlueprintCallable) void CreateLobby();
	UFUNCTION(BlueprintCallable) void DestroySession();

	//user
	UFUNCTION(BlueprintCallable) void LoginEOS(FString ID, FString Token, FString LoginType);
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetPlayerUsername();
	UFUNCTION(BlueprintCallable, BlueprintPure) bool IsLoggedIn();
	
	UPROPERTY(BlueprintAssignable) FOnLoginCompleted OnLoginComplete;
	UPROPERTY(BlueprintAssignable) FStartSessionComplete OnStartSessionComplete;
	UPROPERTY(BlueprintAssignable) FEndSessionComplete OnEndSessionComplete;
	UPROPERTY(BlueprintAssignable) FDestroySessionComplete OnDestroySessionComplete;
	UPROPERTY(BlueprintAssignable) FJoinSessionComplete OnJoinSessionComplete;
	UPROPERTY(BlueprintAssignable) FCreateSessionComplete OnCreateSessionComplete;
	UPROPERTY(BlueprintAssignable) FCreateLobbyComplete OnCreateLobbyComplete;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool bSuccess);
	void OnStartSessionCompleted(FName SessionName, bool bSuccess);
	void OnEndSessionCompleted(FName SessionName, bool bSuccess);
	void OnDestroySessionCompleted(FName SessionName, bool bSuccess);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search);
	void HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);
	void OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);

private:
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	FOnEndSessionCompleteDelegate EndSessionCompleteDelegate;
	FDelegateHandle EndSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	
	void CreateSession(int32 PlayerCount);
	
	FORCEINLINE FOnlineSessionSearchResult GetBestSession(TSharedRef<FOnlineSessionSearch> Search)
	{
		return Search->SearchResults[0];
	}
};




