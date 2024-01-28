#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "Core/MainMenu/MainMenuGameState.h"
#include "UI/Widgets/NetworkErrorWidget.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "Core/Managers/PoolHandlerSubSystem.h"
#include "Core/Other/MapInfo.h"
#include "Managers/CommandHandlerSubsystem.h"
#include "Quests/QuestStatics.h"
#include "ExtractionGameInstance.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginCompleted, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartSessionComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndSessionComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDestroySessionComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FJoinSessionComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateSessionComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateLobbyComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserWriteComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserReadComplete, bool, bWasSuccess);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGetFileComplete, FString, FileName, TArray<uint8>, DataRef);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNetworkError, FName, ErrorName, FString, ErrorDescription);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTravelError, FName, ErrorName, FString, ErrorDescription);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMatchmakingFailed);
UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionAcceptedResult, bool, bWasSuccess);

USTRUCT(BlueprintType)
struct FPlayerSessionData
{
	GENERATED_BODY()

	UPROPERTY()
	bool bIsValid;
	UPROPERTY()
	TMap<int32, FAddItemInfo> PlayerItems;
	UPROPERTY()
	TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GemItems;

	FPlayerSessionData(bool bIsValid, const TMap<int32, FAddItemInfo>& PlayerItems,
		const TMap<TEnumAsByte<EBodyPart>, FAddItemInfo>& GemItems)
		: bIsValid(bIsValid),
		  PlayerItems(PlayerItems),
		  GemItems(GemItems)
	{
	}

	FPlayerSessionData() = default;
};

USTRUCT(BlueprintType)
struct FPlayerRaidResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsValid;
	UPROPERTY(BlueprintReadOnly)
	bool bSurvived;
	UPROPERTY(BlueprintReadOnly)
	float PlayTime;

	FPlayerRaidResult(bool bIsValid, bool bSurvived, float PlayTime)
		: bIsValid(bIsValid),
		  bSurvived(bSurvived),
		  PlayTime(PlayTime)
	{
	}

	FPlayerRaidResult() = default;
};

USTRUCT(BlueprintType)
struct FPlayerQuestData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bIsComplete;
	UPROPERTY(BlueprintReadOnly)
	int32 QuestID;
};



UCLASS()
class EXTRACTIONGAME_API UExtractionGameInstance : public UGameInstance
{
	//TODO: clean up this code, its messy having it all in the game instance because game instance is still the same across levels and you can only interact with sessions in main menu
	GENERATED_BODY()

	
	IOnlineSubsystem* OnlineSubSystem;
	IOnlineIdentityPtr UserIdentity;
	IOnlineSessionPtr Session;
	
	UPROPERTY()UAbilityHandlerSubSystem* AbilityHandlerSubSystem;
	UPROPERTY()UPoolHandlerSubSystem* PoolHandlerSubSystem;
	UPROPERTY()UCommandHandlerSubsystem* CommandHandlerSubsystem;

public:

	UAbilityHandlerSubSystem* GetAbilityHandlerSubSystem() const {return AbilityHandlerSubSystem;}
	
	const int SESSION_PLAYERCOUNT = 4;

	UExtractionGameInstance();
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void SetupOnlineSubsystem();
	const UMapInfo* GetMapInfo();

	UPROPERTY(BlueprintReadOnly)
	FPlayerSessionData PlayerSessionData;
	UPROPERTY(BlueprintReadOnly)
	FPlayerRaidResult PlayerRaidResult;
	

	void BuildPlayerSessionData(TMap<int32, FAddItemInfo> PlayerItems, TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GemItems);
	void OnRaidOver(bool bSurvived, float PlayTime);

	void StartSession();
	void StopSession();
	
	UFUNCTION(BlueprintCallable) void JoinSession(bool bCreateSession);
	UFUNCTION(BlueprintCallable) void CreateLobby();
	UFUNCTION(BlueprintCallable) void DestroySession();
	UFUNCTION(BlueprintCallable) bool TryAutoLogin();
	UFUNCTION(BlueprintCallable) bool LogOut();
	
	UFUNCTION(Exec)
	void SetWantsToHost(bool bWantsToHost);
	

	UFUNCTION(BlueprintImplementableEvent) void ShowLoadingScreen();

	//user
	UFUNCTION(BlueprintCallable) void LoginEOS(FString ID, FString Token, FString LoginType);
	UFUNCTION(BlueprintCallable, BlueprintPure) FString GetPlayerUsername();
	UFUNCTION(BlueprintCallable) void CancelMatchMaking();
	UFUNCTION(BlueprintCallable, BlueprintPure) bool IsLoggedIn();
	
	UPROPERTY(BlueprintAssignable) FOnLoginCompleted OnLoginComplete;
	UPROPERTY(BlueprintAssignable) FStartSessionComplete OnStartSessionComplete;
	UPROPERTY(BlueprintAssignable) FEndSessionComplete OnEndSessionComplete;
	UPROPERTY(BlueprintAssignable) FDestroySessionComplete OnDestroySessionComplete;
	UPROPERTY(BlueprintAssignable) FJoinSessionComplete OnJoinSessionComplete;
	UPROPERTY(BlueprintAssignable) FCreateSessionComplete OnCreateSessionComplete;
	UPROPERTY(BlueprintAssignable) FCreateLobbyComplete OnCreateLobbyComplete;
	UPROPERTY(BlueprintAssignable) FOnUserWriteComplete UserWriteCompleteDelegate;
	UPROPERTY(BlueprintAssignable) FOnUserReadComplete UserReadCompleteDelegate;
	UPROPERTY(BlueprintAssignable) FOnGetFileComplete GetFileCompleteDelegate;
	UPROPERTY(BlueprintAssignable) FOnMatchmakingFailed OnMatchmakingFailedDelegate;
	UPROPERTY(BlueprintAssignable) FOnSessionAcceptedResult OnSessionAcceptedResultDelegate;
	UPROPERTY(BlueprintAssignable) FOnNetworkError OnNetworkErrorDelegate;
	UPROPERTY(BlueprintAssignable) FOnTravelError OnTravelErrorDelegate;
	
	UPROPERTY(EditDefaultsOnly) TMap<FString, UMapInfo*> MapInfo;

	FNamedOnlineSession* CurrentSession;
	FNamedOnlineSession* CurrentLobby;

	bool bCreatedSession;
	bool bLoadedInventoryOnStart;
	bool bFirstTime;
	TArray<uint8> ConvertSavedFileToInt(USaveGame* SavedGame);
	USaveGame* ConvertIntToSavedFile(TArray<uint8> Data);

	void UpdatePlayerData(FString FileName, TArray<uint8> DataRef);
	void ReadPlayerData(FString FileName);
	TArray<FCloudFileHeader>& GetEosPlayerFiles();
	TArray<FCloudFileHeader> CloudFiles;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool bSuccess);
	void OnStartSessionCompleted(FName SessionName, bool bSuccess);
	void OnEndSessionCompleted(FName SessionName, bool bSuccess);
	void OnDestroySessionCompleted(FName SessionName, bool bSuccess);
	void OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search);
	void HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId, FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult);
	void OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID, const FString& Error);
	void HandleNetworkFailure(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString);
	void OnUserWriteComplete(bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error);
	void OnUserReadComplete(bool bWasSuccess, const FUniqueNetId& UserId, const FString& FileName);
	void OnTravelError(UWorld* World, ETravelFailure::Type FailureType, const FString & ErrorString);


	
private:
	void CreateSession(int32 PlayerCount);
	void GetPlayerData(FString FileName);

	bool bHost;
	bool bCurrentlyFindingSessions;
	FORCEINLINE FOnlineSessionSearchResult GetBestSession(TSharedRef<FOnlineSessionSearch> Search)
	{
		FOnlineSessionSearchResult BestSession;
		for(int i = 0; i < Search->SearchResults.Num(); i++)
		{
			if(Search->SearchResults[i].Session.SessionSettings.bUseLobbiesIfAvailable)
			{
				continue;
			}

			BestSession = Search->SearchResults[i];
			break;
		}
		return BestSession;
	}

	FORCEINLINE FString GetNetworkErrorMessage(ENetworkFailure::Type NetworkFailure)
	{
		switch(NetworkFailure)
		{
			case ENetworkFailure::ConnectionTimeout:
				return "Connection Timeout";
			case ENetworkFailure::ConnectionLost:
				return "Connection Lost";
			case ENetworkFailure::NetDriverAlreadyExists:
				return "Net Driver Already Exists";
			case ENetworkFailure::NetDriverCreateFailure:
				return "Net Driver Create Failure";
			case ENetworkFailure::OutdatedClient:
				return "Outdated Client";
			case ENetworkFailure::OutdatedServer:
				return "Outdated Server";
			case ENetworkFailure::PendingConnectionFailure:
				return "Pending Connection Failure";
			case ENetworkFailure::NetGuidMismatch:
				return "Net Guid Mismatch";
			case ENetworkFailure::NetChecksumMismatch:
				return "Net Checksum Mismatch";	
		case ENetworkFailure::FailureReceived:
			return "Failure Received";
			
			default: return "Unknown Network Failure";
		}
	}


	//quests
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FPlayerQuestData> GetPlayerQuests() const { return PlayerQuests; }


	void SetQuestAsComplete(int32 QuestID);
	
private:
	UPROPERTY()
	TArray<FPlayerQuestData> PlayerQuests;
};




