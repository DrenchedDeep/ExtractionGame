#include "ExtractionGameInstance.h"

#include "GameHUD.h"
#include "MainMenuGameState.h"
#include "MainMenuHUD.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "PlayerStashManager.h"
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

void UExtractionGameInstance::BuildPlayerSessionData(TArray<FInventoryItem> PlayerItems, TArray<FName> PartyMembers)
{
	FPlayerSessionData PlayerData(true, PlayerItems, PartyMembers);
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

	const FString RandomPlayerName = TEXT("PLAYER") + FString::FromInt(FMath::RandRange(0, 1000));
	return RandomPlayerName;
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

	Session->OnFindSessionsCompleteDelegates.Clear();
	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnFindSessionCompleted, SessionSearch));


	if (!Session->FindSessions(0, SessionSearch))
	{
		OnMatchmakingFailedDelegate.Broadcast();
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

void UExtractionGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccess)
{
	UE_LOG(LogTemp, Warning, TEXT("session created"));
	CurrentSession = Session->GetNamedSession(SessionName);

	if(!CurrentSession || !bWasSuccess)
	{
		OnCreateSessionComplete.Broadcast(false);
		return;
	}

	FString SessionSettings;
	if(CurrentSession->SessionSettings.Get(SEARCH_KEYWORDS, SessionSettings))
	{
		if(SessionSettings == "PartyLOBBY")
		{
			ShowLoadingScreen();
			GetWorld()->ServerTravel("LVL_MainMenu?listen");
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
