

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
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

public:
	UPROPERTY(BlueprintAssignable)
	FOnLoginCompleted OnLoginComplete;
	
	UFUNCTION(BlueprintCallable)
	void LoginEOS(FString ID, FString Token, FString LoginType);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetPlayerUsername();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLoggedIn();


	UFUNCTION(BlueprintCallable)
	void CreateSession(int32 PlayerCount);

	UFUNCTION(BlueprintCallable)
	void JoinSession();
	
	void OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	void SetupOnlineSubsystem();

	void OnCreateSessionCompleted(FName SessionName, bool bWasSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search);
	void HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
};

