#include "ExtractionGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"


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

	Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnCreateSessionCompleted));
	
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

	if(!Session->CreateSession(0, FName(TEXT("MainSession")), SessionSettings))
	{
		GLog->Log("Error creating session");
		//error
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
	if(bWasSuccess)
	{
		GetWorld()->ServerTravel("SessionMap?listen");
	}
}

void UExtractionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{		
			FString ServerAddress;
			Session->GetResolvedConnectString(SessionName, ServerAddress);
			
			if(!ServerAddress.IsEmpty())
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					FString::Printf(TEXT("%s = StringVariable"), *ServerAddress));
				PlayerController->ClientTravel(ServerAddress + "?listen", ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, TSharedRef<FOnlineSessionSearch> Search)
{
	if (Search->SearchResults.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Found No Sessions!"));
		return;
	}

	if(bWasSuccess)
	{
		if(!Search->SearchResults[0].IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Results r invalid"));
		}
		
		Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
			this,
			&UExtractionGameInstance::OnJoinSessionComplete));

		
		Session->JoinSession(0, FName("MainSession"), Search->SearchResults[0]);
	}
}


#pragma endregion

void UExtractionGameInstance::HandleNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	GLog->Log("Network Error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
}



void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
	GetEngine()->OnNetworkFailure().AddUObject(this, &UExtractionGameInstance::HandleNetworkFailure);
	OnLoginComplete.Broadcast(bWasSuccess);
}
