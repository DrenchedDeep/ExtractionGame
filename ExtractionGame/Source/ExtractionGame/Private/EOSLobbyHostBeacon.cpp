


#include "EOSLobbyHostBeacon.h"

AEOSLobbyHostBeacon::AEOSLobbyHostBeacon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BeaconState = EBeaconState::AllowRequests;
}

bool AEOSLobbyHostBeacon::Start()
{
	IsReady = InitHost();
	
	return IsReady;
}

void AEOSLobbyHostBeacon::AddHost(AOnlineBeaconHostObject* HostObject)
{
	if(IsReady)
	{
		RegisterHost(HostObject);
	}
}
