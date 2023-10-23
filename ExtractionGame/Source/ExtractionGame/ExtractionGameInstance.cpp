#include "ExtractionGameInstance.h"
#include "MainMenuGameState.h"
#include "MainMenuHUD.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
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

	if(!Session->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(),SessionName, SessionSettings))
	{
		
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
			GetWorld()->ServerTravel("Desert_Map?listen");	
			
					if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
            		{
            			//enable loading screen
            			if(AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(PlayerController->GetHUD()))
            			{
            				MainMenuHUD->OnSessionFound();
            			}
            			}
		}
	}
}

void UExtractionGameInstance::OnStartSessionCompleted(FName SessionName, bool bSuccess)
{
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
			//enable loading screen
			if(AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(PlayerController->GetHUD()))
			{
				MainMenuHUD->OnSessionFound();
			}
			
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
			FOnlineSessionSearchResult SearchResult = Search->SearchResults[0];

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
}

void UExtractionGameInstance::HandleSessionInviteAccepted(const bool bWasSuccessful, const int32 ControllerId,
	FUniqueNetIdPtr UserId, const FOnlineSessionSearchResult& InviteResult)
{
	//TODO: check if session is full, if so reject

	
	if(bWasSuccessful)
	{
		DestroySession();
		
		FOnlineSessionSearchResult InviteResultCopy = InviteResult;
		InviteResultCopy.Session.SessionSettings.bUsesPresence = false;

		const FName SessionID = FName(*InviteResult.GetSessionIdStr());
		Session->JoinSession(0, SessionID, InviteResultCopy);
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
	CurrentSession = nullptr;
}


void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
//	GetEngine()->OnNetworkFailure().AddUObject(this, &UExtractionGameInstance::HandleNetworkFailure);
	OnLoginComplete.Broadcast(bWasSuccess);
}
