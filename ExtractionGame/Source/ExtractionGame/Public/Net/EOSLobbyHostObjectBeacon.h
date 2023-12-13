

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHostObject.h"
#include "EOSLobbyHostObjectBeacon.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Transient, NotPlaceable, Config=Engine)
class EXTRACTIONGAME_API AEOSLobbyHostObjectBeacon : public AOnlineBeaconHostObject
{
	GENERATED_BODY()

	virtual AOnlineBeaconClient* SpawnBeaconActor(UNetConnection* ClientConnection) override;
	virtual void OnClientConnected(AOnlineBeaconClient* NewClientActor, UNetConnection* ClientConnection) override;

	virtual bool Init();
	virtual void Unregister() override;

	virtual void DisconnectClient(AOnlineBeaconClient* ClientActor) override;
	virtual void NotifyClientDisconnected(AOnlineBeaconClient* LeavingClientActor) override;

public:
	AEOSLobbyHostObjectBeacon(const FObjectInitializer& ObjectInitializer);
};
