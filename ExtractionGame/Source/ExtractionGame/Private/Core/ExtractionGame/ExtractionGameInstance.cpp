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

void UExtractionGameInstance::AllowVO()
{
	CanTutorialVO = true;
}

void UExtractionGameInstance::VoiceOverCooldown()
{
	CanTutorialVO = false;
	GetTimerManager().ClearTimer(CanVOHandle);
	GetTimerManager().SetTimer(CanVOHandle, this, &UExtractionGameInstance::AllowVO, VODelayLength, false);
}




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


bool UExtractionGameInstance::RemoveFromTotalEssence(float AMT)
{
	bool bSuccess = true;
	
	if(TotalExtractedEssence - AMT < 0)
	{
		bSuccess = false;
	}

	TotalExtractedEssence -= AMT;
	SaveData->SetExtractedEssence(TotalExtractedEssence);

	return bSuccess;
}

bool UExtractionGameInstance::AddToTotalEssence(float AMT)
{
	TotalExtractedEssence += AMT;
	SaveData->SetExtractedEssence(TotalExtractedEssence);

	return true;
}

void UExtractionGameInstance::BuildPlayerSessionData(TMap<int32, FAddItemInfo> PlayerItems,
                                                     TMap<int32, FAddItemInfo> StashItems, TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GemItems)
{
	const FPlayerSessionData PlayerData(true, PlayerItems, GemItems, StashItems);
	PlayerSessionData = PlayerData;
}

void UExtractionGameInstance::BuildPartySessionData(TArray<FString> PlayerNames, int32 TeamID)
{
	const FPartyInfo TempPartyInfo(true, TeamID, PlayerNames);
	PartyInfo = TempPartyInfo;
}

void UExtractionGameInstance::ResetPlayerAndGemInventory()
{
	if(PlayerSessionData.bIsValid)
	{
		PlayerSessionData.GemItems.Reset();
		PlayerSessionData.PlayerItems.Reset();
		
	}
}

void UExtractionGameInstance::OnRaidOver(bool bSurvived, float PlayTime, float ExtractedEssence)
{
	const FPlayerRaidResult RaidResult(true, bSurvived, PlayTime, ExtractedEssence);

	if(SaveData)
	{
		TotalExtractedEssence += ExtractedEssence;
	}
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

	UE_LOG(LogTemp, Warning, TEXT("Joining Session"));

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
	if(CurrentSession)
	{
		DestroySession();
	}
	
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
			UE_LOG(LogTemp, Warning, TEXT("ExtractionGameInstanceCPP --> Sending player to MainMenu"))
			ShowLoadingScreen();
			GetWorld()->ServerTravel("LVL_MainMenu");
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
		FString ServerAddress;
		Session->GetResolvedConnectString(SessionName, ServerAddress);
		SavedAddress = ServerAddress;
		
		for(auto& Pair : CurrentSession->SessionSettings.Settings)
		{
			FString Key = Pair.Key.ToString();
			FString Value = Pair.Value.Data.ToString();
			if(Key == "SEARCHKEYWORDS")
			{
				//if(Value == "PartyLOBBY")
				if(Value == "PartyLOBBY")
				{
					ShowLoadingScreen();
					ConnectToCurrentlySavedSession();
					CurrentLobby = Session->GetNamedSession(SessionName);
				}
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Key: %s, Value: %s"), *Pair.Key.ToString(), *Pair.Value.Data.ToString());
		}
		
		OnJoinSessionComplete.Broadcast(true);
	}
	else
	{
		OnJoinSessionComplete.Broadcast(false);
	}
}


void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search)
{
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
				if(MenuGameState->bIsConnectingToGameSession && MenuGameState->PlayerArray.Num() <= 1)
				{
					const FName SessionName =  FName(SearchResult.GetSessionIdStr());
					Session->JoinSession(0, SessionName, SearchResult);
				}
				//if the party is only one person or the host is sending the clients the connection info
				if(MenuGameState->PartyManager->PartyPlayers.Num() <= 1)
				{
					const FName SessionName =  FName(SearchResult.GetSessionIdStr());
					Session->JoinSession(0, SessionName, SearchResult);
				}
				else
				{
					const FName SessionName = FName(SearchResult.GetSessionIdStr());

					MenuGameState->bIsConnectingToGameSession = true;
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
	const FName SessionID = FName(*InviteResult.GetSessionIdStr());

	//TODO: check if session is full, if so reject invite
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
	GetWorld()->ServerTravel("LVL_MainMenu");

	/*/
	FOnlineSessionSettings SessionSettings;

	const FName LobbyName = FName(FString::FromInt(FMath::RandRange(0, 10000)));
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	
	SessionSettings.bIsDedicated = false;
	SessionSettings.bAllowInvites = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bAllowJoinViaPresenceFriendsOnly = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bAntiCheatProtected = false;
	SessionSettings.bUseLobbiesIfAvailable = false;
	SessionSettings.bAllowJoinInProgress = true;

	const FUniqueNetIdRepl id = LocalPlayer->GetPreferredUniqueNetId();
	SessionSettings.Set(SEARCH_KEYWORDS, FString("PartyLOBBY"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//SessionSettings.Set(SEARCH_KEYWORDS, id.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	 //This is needed to transition from entry level to main menu. If there's an issue with this, then please change LVL_Entry as well.
	 if(!Session->CreateSession(*id, LobbyName, SessionSettings))
	 {
		 OnCreateLobbyComplete.Broadcast(false);
	 }
	 /*/
	 
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

void UExtractionGameInstance::ConnectToCurrentlySavedSession()
{
	if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->ClientTravel(SavedAddress, ETravelType::TRAVEL_Absolute);
		ShowLoadingScreen();
	}
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
