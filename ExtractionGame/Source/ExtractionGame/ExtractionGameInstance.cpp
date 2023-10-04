#include "ExtractionGameInstance.h"

#include "EOSLobbyHostBeacon.h"
#include "EOSLobbyHostObjectBeacon.h"
#include "MainMenuGameState.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "PartyBeaconHost.h"

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

void UExtractionGameInstance::HardJoinSession(FName SessionName, FOnlineSessionSearchResult Result)
{
	Session->JoinSession(0,SessionName, Result);
}

void UExtractionGameInstance::Init()
{
	Super::Init();

	AbilityHandlerSubSystem = GetSubsystem<UAbilityHandlerSubSystem>();
	if (!AbilityHandlerSubSystem)
	{
		AbilityHandlerSubSystem = NewObject<UAbilityHandlerSubSystem>(this);
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
	&UExtractionGameInstance::OnJoinSessionComplete));
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

	if(Token.IsEmpty())
	{
		UserAccount.Type = "accountportal";
	}

	UserIdentity->OnLoginCompleteDelegates->AddUObject(this, &UExtractionGameInstance::OnLoginCompleted);
	UserIdentity->Login(0, UserAccount);
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

	return FString();
}

bool UExtractionGameInstance::IsLoggedIn()
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}

	return UserIdentity->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}



#pragma region Sessions

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
	
	bool bSuccess = Session->CreateSession(0,SessionName, SessionSettings);

	if(bSuccess)
	{
		if(AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState()))
		{
		//	FOnlineSessionSearchResult TempResult;
		///	const FClientConnectionInfo ConnectionInfo(true,SessionName, TempResult);
		//	MenuGameState->Multicast_SendConnectionInfo(ConnectionInfo);
		}
	}
}

void UExtractionGameInstance::JoinSession()
{
	if(Session == nullptr)
	{
		Session = OnlineSubSystem->GetSessionInterface();
	}

	TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>();
	
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->QuerySettings.Set(
		SEARCH_KEYWORDS,
		FString("GameplaySession"),
		EOnlineComparisonOp::Equals);
	
	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnFindSessionCompleted, SessionSearch));

	
	if (!Session->FindSessions(0, SessionSearch))
	{
	}
}

void UExtractionGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccess)
{
	CurrentSession = Session->GetNamedSession(SessionName);

	if(!CurrentSession || !bWasSuccess)
	{
		return;
	}

	FString SessionSettings;
	if(CurrentSession->SessionSettings.Get(SEARCH_KEYWORDS, SessionSettings))
	{
		if(SessionSettings == "PartyLOBBY")
		{
			GetWorld()->ServerTravel("LVL_MainMenu?listen");
		}
		else if(SessionSettings == "GameplaySession")
		{
			GetWorld()->ServerTravel("SessionMap?listen");
		}
		else if(SessionSettings == "MatchMakingLOBBY")
		{
		}
	}
}


void UExtractionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		CurrentSession = Session->GetNamedSession(SessionName);

		if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			FString ServerAddress;
			Session->GetResolvedConnectString(SessionName, ServerAddress);

			PlayerController->ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search)
{
	AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState());

	if(!MenuGameState)
	{
		return;
	}

	
	if(Search->SearchResults.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found No Sessions"));
		if(MenuGameState->PartyManager->GetLocalPartyPlayer().bIsHost)
		{
			CreateSession(SESSION_PLAYERCOUNT);
		}
		else
		{
		//	JoinSession();
		}
		return;
	}

	if(bWasSuccess)
	{
		if(MenuGameState->PartyManager->GetLocalPartyPlayer().bIsHost)
		{
			UE_LOG(LogTemp, Warning, TEXT("PARTY HOST"));
			FOnlineSessionSearchResult SearchResult = Search->SearchResults[0];

			if(SearchResult.IsSessionInfoValid())
			{
				const FName SessionName = FName(SearchResult.GetSessionIdStr());

				FClientConnectionInfo ConnectInfo(true, SessionName);
				MenuGameState->Multicast_JoinSession(ConnectInfo);

				FTimerHandle UniqueHandle;
				FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &UExtractionGameInstance::HardJoinSession, SessionName, SearchResult);
				GetWorld()->GetTimerManager().SetTimer( UniqueHandle, RespawnDelegate, 5.f, false );
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("PARTY CLIENT"));
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
	
	
	/*/
	if (Search->SearchResults.Num() <= 0 && bIsPartyHost)
	{
		UE_LOG(LogTemp, Warning, TEXT("making session"));
		CreateSession(4);
		return;
	}


	//if we are joining a session, we need to let all the clients in the party know the connection info
	if(bWasSuccess)
	{
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
			this,
			&UExtractionGameInstance::OnJoinSessionComplete));
		
		const FName SessionName(Search->SearchResults[0].GetSessionIdStr());
	
		if(bIsPartyHost)
		{
			UE_LOG(LogTemp, Warning, TEXT("party host"));
			if(AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState()))
			{
				const int SessionPlayerCount = Search->SearchResults[0].Session.NumOpenPublicConnections;

				if(SessionPlayerCount + MenuGameState->PlayerArray.Num() > 4)
				{
					return;
				}

				UE_LOG(LogTemp, Warning, TEXT("session player count is ok"));


				//connecting to session logic
				const FClientConnectionInfo ConnectionInfo(true, SessionName, Search->SearchResults[0]);
				MenuGameState->Multicast_JoinSession(ConnectionInfo);

				
				FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject( this, &UExtractionGameInstance::DelaySessionJoin, ConnectionInfo);
				GetWorld()->GetTimerManager().SetTimer(DelayJoinSession, TimerDelegate, 5.f, false);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("joining session as client"));
			if(const AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState()))
			{
				if(MenuGameState->ConnectionInfo.bIsValid)
				{
					Session->JoinSession(0, MenuGameState->ConnectionInfo.SessionName, Search->SearchResults[0]);
				}
			}
		}
	}
	else
	{
		if(bIsPartyHost)
		{
			UE_LOG(LogTemp, Warning, TEXT("making session"));
			CreateSession(10);
		}
		else
		{
			//if the party members cant find the session, we need to retry or log an error to the client altho that is not ideal
		}
	}
	/*/
}

#pragma endregion


void UExtractionGameInstance::HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	//check if session is full, if so reject 
	if(bWasSuccessful)
	{
		DestroySession();
		
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
	this,
	&UExtractionGameInstance::OnJoinSessionComplete));

		FOnlineSessionSearchResult InviteResultCopy = InviteResult;
		InviteResultCopy.Session.SessionSettings.bUsesPresence = false;
		
		FName SessionID = FName(*InviteResult.GetSessionIdStr());
		Session->JoinSession(0, SessionID, InviteResultCopy);
	}
}



void UExtractionGameInstance::Shutdown()
{
	Super::Shutdown();

	DestroySession();
}

bool UExtractionGameInstance::CreateLobby()
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
	return Session->CreateSession(0, LobbyName, SessionSettings);
}

void UExtractionGameInstance::DestroySession()
{
	if(!CurrentSession)
	{
		return;
	}

	Session->DestroySession(CurrentSession->SessionName);
	CurrentSession = nullptr;
}

bool UExtractionGameInstance::CreateMatchMakingLobby()
{
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsDedicated = false;
	SessionSettings.bAllowInvites = true;
	SessionSettings.NumPublicConnections = SESSION_PLAYERCOUNT;
	SessionSettings.bUsesPresence = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAntiCheatProtected = false;
	SessionSettings.bAllowJoinInProgress = true;
	
	SessionSettings.Set(SEARCH_KEYWORDS, FString("MatchMakingLOBBY"), EOnlineDataAdvertisementType::ViaOnlineService);
	
	const FName LobbyName = "MatchmakerLobby";
	return Session->CreateSession(0, LobbyName, SessionSettings);
}


void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
//	GetEngine()->OnNetworkFailure().AddUObject(this, &UExtractionGameInstance::HandleNetworkFailure);
	OnLoginComplete.Broadcast(bWasSuccess);
}
