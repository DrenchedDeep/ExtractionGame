


#include "EOSLobbyClientBeacon.h"


AEOSLobbyClientBeacon::AEOSLobbyClientBeacon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


void AEOSLobbyClientBeacon::OnFailure()
{
	Super::OnFailure();

	UE_LOG(LogTemp, Warning, TEXT("Failed to connect to server"));
}

bool AEOSLobbyClientBeacon::StartBeacon(FString Address, int32 Port, const bool PortOverride)
{
	FURL URl(nullptr, *Address, TRAVEL_Absolute);

	if(PortOverride)
	{
		URl.Port = Port;
	}
	else
	{
		int32 PortConfig;
		GConfig->GetInt(TEXT("/Script/OnlineSubsystemUtils.OnlineBeaconHost"), TEXT("ListenPort"), PortConfig, GEngineIni);
		URl.Port = PortConfig;
	}


	return InitClient(URl);
}

void AEOSLobbyClientBeacon::DisconnectBeacon()
{
//	DisconnectBeacon();
}

