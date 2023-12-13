


#include "Net/EOSLobbyHostObjectBeacon.h"

#include "Net/EOSLobbyClientBeacon.h"
#include "OnlineBeaconClient.h"


AEOSLobbyHostObjectBeacon::AEOSLobbyHostObjectBeacon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ClientBeaconActorClass = AEOSLobbyClientBeacon::StaticClass();
	BeaconTypeName = ClientBeaconActorClass->GetName();

	PrimaryActorTick.bCanEverTick = true;
	//if we add dedicated servers down the line
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

AOnlineBeaconClient* AEOSLobbyHostObjectBeacon::SpawnBeaconActor(UNetConnection* ClientConnection)
{
	return Super::SpawnBeaconActor(ClientConnection);
}

void AEOSLobbyHostObjectBeacon::OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection)
{
	Super::OnClientConnected(NewClientActor, ClientConnection);

	AEOSLobbyClientBeacon* ClientBeacon = Cast<AEOSLobbyClientBeacon>(NewClientActor);

	if(ClientBeacon)
	{
		//send rpc to client to let them know they joined succesfully
		UE_LOG(LogTemp, Warning, TEXT("Client Joined Lobby Succesfully"));
	}
}

bool AEOSLobbyHostObjectBeacon::Init()
{
	return true;
}

void AEOSLobbyHostObjectBeacon::Unregister()
{
	Super::Unregister();
}

void AEOSLobbyHostObjectBeacon::DisconnectClient(AOnlineBeaconClient* ClientActor)
{
	Super::DisconnectClient(ClientActor);
}

void AEOSLobbyHostObjectBeacon::NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor)
{
	Super::NotifyClientDisconnected(LeavingClientActor);
}

