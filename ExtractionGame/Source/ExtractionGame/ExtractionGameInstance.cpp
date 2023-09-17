


#include "ExtractionGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineSessionSettings.h"
#include "Kismet/GameplayStatics.h"

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


	Session = OnlineSubSystem->GetSessionInterface();

	Session->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnCreateSessionCompleted));
	
	if(Session == nullptr)
	{
		return;
	}
	
	FOnlineSessionSettings SessionSettings;

	//will have to change when we add in dedicated server support
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bAllowInvites = true;
	SessionSettings.NumPublicConnections = PlayerCount;
	SessionSettings.bUseLobbiesIfAvailable = false;
	SessionSettings.bShouldAdvertise = true;

	SessionSettings.Settings.Add(
FName(TEXT("SessionSetting")),
FOnlineSessionSetting(FString(TEXT("SettingValue")), EOnlineDataAdvertisementType::ViaOnlineService));

	if(!Session->CreateSession(0, FName(TEXT("Session1")), SessionSettings))
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

	/*/
	//search for listen servers and dedicated servers
	SessionSearch->QuerySettings.Set(
		FName(TEXT("__EOS_bListening")),
		false,
		EOnlineComparisonOp::Equals);

	/*/
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->bIsLanQuery = false;
	SessionSearch->QuerySettings.SearchParams.Empty();

	Session->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsComplete::FDelegate::CreateUObject(
		this,
		&UExtractionGameInstance::OnFindSessionCompleted, SessionSearch.Get()));

	Session->FindSessions(0, SessionSearch);
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

void UExtractionGameInstance::OnCreateSessionCompleted(FName SessionName, bool bWasSuccess)
{
	if(bWasSuccess)
	{
		GetWorld()->ServerTravel("SessionMap");
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
				PlayerController->ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute);
			}
		}
	}
}

void UExtractionGameInstance::OnFindSessionCompleted(bool bWasSuccess, FOnlineSessionSearch Search)
{
	if(bWasSuccess)
	{
		if(Search.SearchResults[0].IsValid())
		{
			Session->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
					  this,
					  &UExtractionGameInstance::OnJoinSessionComplete));
		
			//join first session from search results
			Session->JoinSession(0, FName("Session1"), Search.SearchResults[0]);
		}
		else
		{
			//remove later
			CreateSession(2);
		}
	}
	else
	{
		//remove later
		CreateSession(2);
	}
}


void UExtractionGameInstance::OnLoginCompleted(int32 LocalUser, bool bWasSuccess, const FUniqueNetId& UserID,
                                               const FString& Error)
{
	OnLoginComplete.Broadcast(bWasSuccess);
}
