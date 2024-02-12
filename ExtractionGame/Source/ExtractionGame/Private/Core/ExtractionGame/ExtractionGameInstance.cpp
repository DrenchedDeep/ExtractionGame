#include "Core/ExtractionGame/ExtractionGameInstance.h"

#include "UI/GameHUD.h"
#include "Core/MainMenu/MainMenuGameState.h"
#include "UI/MainMenuHUD.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Core/Managers/PlayerStashManager.h"
#include "GameFramework/SaveGame.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

UExtractionGameInstance::UExtractionGameInstance()
{
	
}

void UExtractionGameInstance::SetupOnlineSubsystem()
{
	if(IOnlineSubsystem* SubSystem = Online::GetSubsystem(GetWorld()); SubSystem != nullptr)
	{ 
		OnlineSubSystem = SubSystem;

		const IOnlineIdentityPtr Identity = SubSystem->GetIdentityInterface();
		
		if(Identity != nullptr)
		{
			UserIdentity = Identity;
		}
	}
}

const UMapInfo* UExtractionGameInstance::GetMapInfo()
{
	checkf(GetWorld(), TEXT("Cannot get map info before a map exists"))
	UE_LOG(LogTemp,Warning,TEXT("WORLD: %s"), *GetWorld()->GetName());
	return MapInfo[GetWorld()->GetName()];
}

void UExtractionGameInstance::BuildPlayerSessionData(TMap<int32, FAddItemInfo> PlayerItems, TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GemItems)
{
	FPlayerSessionData PlayerData(true, PlayerItems, GemItems);
	PlayerSessionData = PlayerData;
}

void UExtractionGameInstance::OnRaidOver(bool bSurvived, float PlayTime)
{
	const FPlayerRaidResult RaidResult(true, bSurvived, PlayTime);

	PlayerRaidResult = RaidResult;
}

void UExtractionGameInstance::StartSession()
{
	if(CurrentSession)
	{
		Session->StartSession(CurrentSession->SessionName);
	}
}

void UExtractionGameInstance::StopSession()
{
	if(CurrentSession)
	{
		Session->EndSession(CurrentSession->SessionName);
	}
}


void UExtractionGameInstance::Init()
{
	Super::Init();


	
	AbilityHandlerSubSystem = GetSubsystem<UAbilityHandlerSubSystem>();
	if (!AbilityHandlerSubSystem)
	{
		AbilityHandlerSubSystem = NewObject<UAbilityHandlerSubSystem>(this);
	}

	PoolHandlerSubSystem = GetSubsystem<UPoolHandlerSubSystem>();
	if (!PoolHandlerSubSystem)
	{
		PoolHandlerSubSystem = NewObject<UPoolHandlerSubSystem>(this);
	}

	CommandHandlerSubsystem = GetSubsystem<UCommandHandlerSubsystem>();
	if (!CommandHandlerSubsystem)
	{
		CommandHandlerSubsystem = NewObject<UCommandHandlerSubsystem>(this);
		CommandHandlerSubsystem->Init(AbilityHandlerSubSystem);
	}
	
	

	SetupOnlineSubsystem();
	Session = OnlineSubSystem->GetSessionInterface();


	Session->AddOnSessionUserInviteAcceptedDelegate_Handle(FOnSessionUserInviteAccepted::FDelegate::CreateUObject(
	this,
	&UExtractionGameInstance::HandleSessionInviteAccepted));

	Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnCreateSessionCompleted));

	Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
	this,
	&UExtractionGameInstance::OnJoinSessionCompleted));
	
	Session->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnDestroySessionCompleted));

	Session->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionComplete::FDelegate::CreateUObject(
	this,
	&UExtractionGameInstance::OnStartSessionCompleted));

	Session->AddOnEndSessionCompleteDelegate_Handle(FOnEndSessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnEndSessionCompleted));


	UserIdentity->OnLoginCompleteDelegates->AddUObject(this, &UExtractionGameInstance::OnLoginCompleted);

	IOnlineUserCloudPtr UserCloud = OnlineSubSystem->GetUserCloudInterface();

	if(UserCloud)
	{
		UserCloud->OnWriteUserFileCompleteDelegates.AddUObject(this, &UExtractionGameInstance::OnUserWriteComplete);
		UserCloud->OnReadUserFileCompleteDelegates.AddUObject(this, &UExtractionGameInstance::OnUserReadComplete);
	}
	
	GetEngine()->OnNetworkFailure().AddUObject(this, &UExtractionGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UExtractionGameInstance::OnTravelError);

	
}


void UExtractionGameInstance::LoginEOS(FString ID, FString Token, FString LoginType)
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}
	
	FOnlineAccountCredentials UserAccount;
	UserAccount.Id = ID;
	UserAccount.Token = Token;
	UserAccount.Type = LoginType;
	if(LoginType.IsEmpty())
	{
		UserAccount.Type = "accountportal";
	}
	UserIdentity->Login(0, UserAccount);
	/*/
	//bool bAutoLogin = UserIdentity->Login(0, UserAccount);
	
	if(bAutoLogin)
	{
		UE_LOG(LogTemp, Warning, TEXT("HI"));
		UserAccount.Type = "accountportal";
		UserIdentity->Login(0, UserAccount);
	}
	/*/
}

FString UExtractionGameInstance::GetPlayerUsername()
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}

	if(UserIdentity->GetLoginStatus(0) == ELoginStatus::LoggedIn)
	{
		return UserIdentity->GetPlayerNickname(0);
	}

	const FString RandomPlayerName = TEXT("PLAYER") + FString::FromInt(FMath::RandRange(0, 1000));
	return RandomPlayerName;
}

void UExtractionGameInstance::CancelMatchMaking()
{
	if(bCurrentlyFindingSessions)
	{
		Session->CancelFindSessions();
		bCurrentlyFindingSessions = false;
	}
}

bool UExtractionGameInstance::IsLoggedIn()
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}

	return UserIdentity->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}


void UExtractionGameInstance::CreateSession(int32 PlayerCount)
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}

	if(Session == nullptr)
	{
		return;
	}
	
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsDedicated = false;
	SessionSettings.bAllowInvites = false;
	SessionSettings.NumPublicConnections = PlayerCount;
	SessionSettings.bUseLobbiesIfAvailable = false;
	SessionSettings.bUsesPresence = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings.bAllowJoinViaPresence = false;
	SessionSettings.bAntiCheatProtected = false;
	
	SessionSettings.Settings.Add(
		SEARCH_KEYWORDS,
		FOnlineSessionSetting(FString(TEXT("GameplaySession")), EOnlineDataAdvertisementType::ViaOnlineService));

	
	const FName SessionName = FName(FString::FromInt(FMath::RandRange(0, 10000)));
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	UE_LOG(LogTemp, Warning, TEXT("session created"));

	if(!bCreatedSession)
	{
		bCreatedSession = true;
		if(!Session->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),SessionName, SessionSettings))
		{
			OnCreateSessionComplete.Broadcast(false);
		}
	}
}

void UExtractionGameInstance::ReadPlayerData(FString FileName)
{
	IOnlineUserCloudPtr UserCloud = OnlineSubSystem->GetUserCloudInterface();

	if(UserCloud)
	{
		const TSharedPtr<const FUniqueNetId> UserIDRef =  UserIdentity->GetUniquePlayerId(0).ToSharedRef();
		UserCloud->ReadUserFile(*UserIDRef, FileName);
	}
}

TArray<FCloudFileHeader>& UExtractionGameInstance::GetEosPlayerFiles()
{
	TArray<FCloudFileHeader>& Files = CloudFiles;
	IOnlineUserCloudPtr UserCloud = OnlineSubSystem->GetUserCloudInterface();
	
	const TSharedPtr<const FUniqueNetId> UserIDRef =  UserIdentity->GetUniquePlayerId(0).ToSharedRef();
	UserCloud->GetUserFileList(*UserIDRef, Files);
	
	return Files;
}

void UExtractionGameInstance::JoinSession(bool bCreateSession)
{
	if(bCreateSession || bHost)
	{
		CreateSession(SESSION_PLAYERCOUNT);
		return;
	}
	
	if(Session == nullptr)
	{
		Session = OnlineSubSystem->GetSessionInterface();
	}

	if(CurrentSession)
	{
		DestroySession();
	}

	TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>();
	
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->QuerySettings.Set(
		SEARCH_KEYWORDS,
		FString("GameplaySession"),
		EOnlineComparisonOp::Equals);

	Session->OnFindSessionsCompleteDelegates.Clear();
	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnFindSessionCompleted, SessionSearch));

	bCurrentlyFindingSessions = true;
	if (!Session->FindSessions(0, SessionSearch))
	{
		OnJoinSessionComplete.Broadcast(false);
	}
}

TArray<uint8> UExtractionGameInstance::ConvertSavedFileToInt(USaveGame* SavedGame)
{
	if(SavedGame)
	{
		TArray<uint8> LocalDataRef;
		UGameplayStatics::SaveGameToMemory(SavedGame, LocalDataRef);

		return LocalDataRef;
	}

	return TArray<uint8>();
}

USaveGame* UExtractionGameInstance::ConvertIntToSavedFile(TArray<uint8> Data)
{
	USaveGame* LocalObject = nullptr;

	if(!Data.IsEmpty())
	{
		LocalObject = UGameplayStatics::LoadGameFromMemory(Data);
	}

	return LocalObject;
}

void UExtractionGameInstance::UpdatePlayerData(FString FileName, TArray<uint8> DataRef)
{
	if(!DataRef.IsEmpty())
	{
		IOnlineUserCloudPtr UserCloud = OnlineSubSystem->GetUserCloudInterface();

		if(UserCloud)
		{
			const TSharedPtr<const FUniqueNetId> UserIDRef =  UserIdentity->GetUniquePlayerId(0).ToSharedRef();
			UserCloud->WriteUserFile(*UserIDRef, FileName, DataRef);
		}
	}
}

void UExtractionGameInstance::GetPlayerData(FString FileName)
{
	IOnlineUserCloudPtr UserCloud = OnlineSubSystem->GetUserCloudInterface();

	if(UserCloud)
	{
		TSharedPtr<const FUniqueNetId> UserIDRef =  UserIdentity->GetUniquePlayerId(0).ToSharedRef();

		TArray<uint8> FileContents;
		UserCloud->GetFileContents(*UserIDRef, FileName, FileContents);
		GetFileCompleteDelegate.Broadcast(FileName, FileContents);
	}
}

void UExtractionGameInstance::SetQuestAsComplete(int32 QuestID)
{
	for(int32 i = 0; i < PlayerQuests.Num(); i++)
	{
		const int32 ArrayQuestID = PlayerQuests[i].QuestID;
		if(QuestID == ArrayQuestID)
		{
			PlayerQuests[i].bIsComplete = true;
		}
	}
}

bool UExtractionGameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	UE_LOG(LogTemp,Warning,TEXT("Received Exec"))
	bool handled = Super::ProcessConsoleExec(Cmd, Ar, Executor);
	if (!handled)
	{
		UE_LOG(LogTemp,Warning,TEXT("My Exec"))
		handled = CommandHandlerSubsystem->ProcessConsoleExec(Cmd, Ar, Executor);
	}
	return handled;
}

void UExtractionGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccess)
{
	CurrentSession = Session->GetNamedSession(SessionName);

	if(!CurrentSession || !bWasSuccess)
	{
		OnCreateSessionComplete.Broadcast(false);
		OnJoinSessionComplete.Broadcast(false);
		return;
	}

	FString SessionSettings;
	if(CurrentSession->SessionSettings.Get(SEARCH_KEYWORDS, SessionSettings))
	{
		if(SessionSettings == "PartyLOBBY")
		{
			ShowLoadingScreen();
			GetWorld()->ServerTravel("LVL_MainMenu?listen");
			CurrentLobby = Session->GetNamedSession(SessionName);
		}
		else if(SessionSettings == "GameplaySession")
		{
			ShowLoadingScreen();
			GetWorld()->ServerTravel("Desert_Map?listen");
		}

		OnCreateSessionComplete.Broadcast(true);
	}
}

void UExtractionGameInstance::OnStartSessionCompleted(FName SessionName, bool bSuccess)
{
	if(!bSuccess)
	{
	}
	
	OnStartSessionComplete.Broadcast(bSuccess);
}

void UExtractionGameInstance::OnEndSessionCompleted(FName SessionName, bool bSuccess)
{
	OnEndSessionComplete.Broadcast(bSuccess);
}

void UExtractionGameInstance::OnDestroySessionCompleted(FName SessionName, bool bSuccess)
{
	OnDestroySessionComplete.Broadcast(bSuccess);
}

void UExtractionGameInstance::OnJoinSessionCompleted(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		CurrentSession = Session->GetNamedSession(SessionName);

		if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			ShowLoadingScreen();

			FString ServerAddress;
			Session->GetResolvedConnectString(SessionName, ServerAddress);
			OnJoinSessionComplete.Broadcast(true);

			PlayerController->ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		OnJoinSessionComplete.Broadcast(false);
	}
}


void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search)
{
	//first problem:
	/*/ when a host crashes, the session is still there, but the host is gone therefore terminating the listen server, but doesnt destroy session so it shows up in findsession
	 * 2nd problem:
	 * clients need to wait for the host to create the session, then join it, so if they all hit accept match at once, they will all create their own sessions
	 */



	/*/
	 *  if there are no servers to join, throw players into a matchmaking lobby instead of immediatly creating a session
	 *  matchmaking lobby will wait till filled up, then create a session based on the best host and send all players to the session
	 */


	bCurrentlyFindingSessions = false;
	AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState());

	if(!MenuGameState)
	{
		OnJoinSessionComplete.Broadcast(false);
		return;
	}
	
	if(Search->SearchResults.Num() <= 0)
	{
		if(MenuGameState->PartyManager->GetLocalPartyPlayer().bIsHost)
		{
			JoinSession(false);
		}
		
		return;
	}

	if(bWasSuccess)
	{
		if(MenuGameState->PartyManager->GetLocalPartyPlayer().bIsHost)
		{
			FOnlineSessionSearchResult SearchResult = GetBestSession(Search);

			if(SearchResult.IsSessionInfoValid())
			{
				if(MenuGameState->PartyManager->PartyPlayers.Num() <= 1)
				{
					//if were the party host, just join directly
					const FName SessionName =  FName(SearchResult.GetSessionIdStr());
					Session->JoinSession(0, SessionName, SearchResult);
				}
				else
				{
					const FName SessionName = FName(SearchResult.GetSessionIdStr());

					FClientConnectionInfo ConnectInfo(true, SessionName);
					MenuGameState->Multicast_JoinSession(ConnectInfo);
				}
			}
			else
			{
				OnJoinSessionComplete.Broadcast(false);
			}
		}
		else
		{
			if(MenuGameState->ConnectionInfo.bIsValid)
			{
				for(int32 i = 0; i < Search->SearchResults.Num(); i++)
				{
					if(MenuGameState->ConnectionInfo.SessionName == Search->SearchResults[i].GetSessionIdStr())
					{
						const FName SessionName = FName(Search->SearchResults[i].GetSessionIdStr());
						Session->JoinSession(0, SessionName, Search->SearchResults[i]);
						break;
					}
				}
			}
		}
	}
	else
	{
		OnMatchmakingFailedDelegate.Broadcast();
	}
}

void UExtractionGameInstance::HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	//TODO: check if session is full, if so reject invite
	const FName SessionID = FName(*InviteResult.GetSessionIdStr());

	if(Session->GetNamedSession(SessionID) && Session->GetNamedSession(SessionID)->NumOpenPublicConnections >= 2)
	{
		OnSessionAcceptedResultDelegate.Broadcast(false);
		return;
	}
	
	if(bWasSuccessful)
	{
		DestroySession();
		
		FOnlineSessionSearchResult InviteResultCopy = InviteResult;
		InviteResultCopy.Session.SessionSettings.bUsesPresence = false;

		Session->JoinSession(0, SessionID, InviteResultCopy);
		OnSessionAcceptedResultDelegate.Broadcast(true);
	}
}



void UExtractionGameInstance::Shutdown()
{
	Super::Shutdown();

	DestroySession();
}

void UExtractionGameInstance::CreateLobby()
{
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsDedicated = false;
	SessionSettings.bAllowInvites = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAntiCheatProtected = false;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.Set(SEARCH_KEYWORDS, FString("PartyLOBBY"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
 
	const FName LobbyName = FName(FString::FromInt(FMath::RandRange(0, 10000)));
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	 if(!Session->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), LobbyName, SessionSettings))
	 {
		 OnCreateLobbyComplete.Broadcast(false);
	 }
}

void UExtractionGameInstance::DestroySession()
{
	if(!CurrentSession)
	{
		return;
	}

	Session->DestroySession(CurrentSession->SessionName);
	bCreatedSession = false;
	CurrentSession = nullptr;
}

bool UExtractionGameInstance::TryAutoLogin()
{
	FOnlineAccountCredentials UserAccount;
	UserAccount.Id = FString();
	UserAccount.Token = FString();
	UserAccount.Type = "persistentauth";
	
	return  UserIdentity->Login(0, UserAccount);
}

bool UExtractionGameInstance::LogOut()
{
	if(!IsLoggedIn())
	{
		return false;
	}

	if(CurrentSession)
	{
		DestroySession();
	}

	if(CurrentLobby)
	{
		Session->DestroySession(CurrentLobby->SessionName);
		CurrentLobby = nullptr;
	}

	return UserIdentity->Logout(0);
}

void UExtractionGameInstance::SetWantsToHost(bool bWantsToHost)
{
	bHost = bWantsToHost;
}


void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
	OnLoginComplete.Broadcast(bWasSuccess);
}

void UExtractionGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver,
	ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	OnNetworkErrorDelegate.Broadcast(ToString(FailureType), ErrorString);
}

void UExtractionGameInstance::OnTravelError(UWorld* World, ETravelFailure::Type FailureType,
	const FString& ErrorString)
{
	OnTravelErrorDelegate.Broadcast(ToString(FailureType), ErrorString);
}



void UExtractionGameInstance::OnUserWriteComplete(bool bWasSuccess, const FUniqueNetId& UserId, const FString& Error)
{
	UserWriteCompleteDelegate.Broadcast(bWasSuccess);
}

void UExtractionGameInstance::OnUserReadComplete(bool bWasSuccess, const FUniqueNetId& UserId, const FString& FileName)
{
	if(bWasSuccess)
	{
		GetPlayerData(FileName);
	}

	UserReadCompleteDelegate.Broadcast(bWasSuccess);
}
