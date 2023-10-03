#include "ExtractionGameInstance.h"

#include "MainMenuGameState.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlinePartyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"


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

//server
void UExtractionGameInstance::DelaySessionJoin(const FClientConnectionInfo ConnectionInfo)
{
	DestroySession();
	Session->JoinSession(0, ConnectionInfo.SessionName,ConnectionInfo.SearchResult);
}



void UExtractionGameInstance::Init()
{
	Super::Init();

	AbilityHandlerSubSystem = GetSubsystem<UAbilityHandlerSubSystem>();
	if (!AbilityHandlerSubSystem)
	{
		AbilityHandlerSubSystem = NewObject<UAbilityHandlerSubSystem>(this);
	}
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
	
	Session = OnlineSubSystem->GetSessionInterface();

//	Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
	//	this,
	//	&UExtractionGameInstance::OnCreateSessionCompleted));

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
		FName(TEXT("SessionSetting")),
		FOnlineSessionSetting(FString(TEXT("SettingValue")), EOnlineDataAdvertisementType::ViaOnlineService));


	const FName SessionName = FName(FString::FromInt(FMath::RandRange(0, 10000)));
	
	bool bSuccess = Session->CreateSession(0,SessionName, SessionSettings);

	if(bSuccess)
	{
		if(AMainMenuGameState* MenuGameState = Cast<AMainMenuGameState>(GetWorld()->GetGameState()))
		{
			FOnlineSessionSearchResult TempResult;
			const FClientConnectionInfo ConnectionInfo(true,SessionName, TempResult);
			MenuGameState->Multicast_SendConnectionInfo(ConnectionInfo);
		}
	}
}

void UExtractionGameInstance::JoinSession(const FClientConnectionInfo ConnectionInfo)
{
	if(const FNamedOnlineSession* SessionSettings = Session->GetNamedSession(ConnectionInfo.SessionName))
	{
		Session->JoinSession(0, SessionSettings->SessionName, ConnectionInfo.SearchResult);
	}
}

void UExtractionGameInstance::JoinSession()
{
	if(Session == nullptr)
	{
		Session = OnlineSubSystem->GetSessionInterface();
	}

	TSharedRef<FOnlineSessionSearch> SessionSearch = MakeShared<FOnlineSessionSearch>();

	SessionSearch->QuerySettings.Set(
		FName(TEXT("__EOS_bListening")),
		false,
		EOnlineComparisonOp::Equals);

	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->QuerySettings.SearchParams.Empty();

	SessionSearch->QuerySettings.Set(
		FName(TEXT("SessionSetting")),
		FString(TEXT("SettingValue")),
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
	const FOnlineSessionSettings* SessionSettings = Session->GetSessionSettings(SessionName);
	
	if(!SessionSettings || !bWasSuccess)
	{
		return;
	}


	//session is a lobby, dont load gameplay map
	if(SessionSettings->bAllowInvites)
	{
		GetWorld()->ServerTravel("LVL_MainMenu?listen");
		bIsPartyHost = true;
	}
	else
	{
		GetWorld()->ServerTravel("LVL_SessionTransition?listen");
		bIsPartyHost = false;
	}

	CurrentSession = Session->GetNamedSession(SessionName);
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

			//we cant be a host of a party if were travelling to a new ip
			if(bIsPartyHost)
			{
				bIsPartyHost = false;
			}
		}
	}
}

void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search)
{
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
}

#pragma endregion


void UExtractionGameInstance::HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	//check if session is full, if so reject 
	if(bWasSuccessful)
	{
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
	this,
	&UExtractionGameInstance::OnJoinSessionComplete));

		
		FName SessionID = FName(*InviteResult.GetSessionIdStr());
		Session->JoinSession(0, SessionID, InviteResult);
	}
}



void UExtractionGameInstance::Shutdown()
{
	Super::Shutdown();

	DestroySession();
}

bool UExtractionGameInstance::CreateLobby()
{
	if(OnlineSubSystem == nullptr || UserIdentity == nullptr)
	{
		SetupOnlineSubsystem();
	}


	Session = OnlineSubSystem->GetSessionInterface();
	
	Session->AddOnSessionUserInviteAcceptedDelegate_Handle(FOnSessionUserInviteAccepted::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::HandleSessionInviteAccepted));

	Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnCreateSessionCompleted));

	if(Session == nullptr)
	{
		return false;
	}
	
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

	SessionSettings.Set(SEARCH_KEYWORDS, FString("LOBBY"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
 
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


void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
//	GetEngine()->OnNetworkFailure().AddUObject(this, &UExtractionGameInstance::HandleNetworkFailure);
	OnLoginComplete.Broadcast(bWasSuccess);
}
