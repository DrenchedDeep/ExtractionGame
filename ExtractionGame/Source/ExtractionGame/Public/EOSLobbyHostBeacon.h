

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconHost.h"
#include "EOSLobbyHostBeacon.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AEOSLobbyHostBeacon : public AOnlineBeaconHost
{
	GENERATED_BODY()

public:
	AEOSLobbyHostBeacon(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintCallable)
	bool Start();

	UFUNCTION(BlueprintCallable)
	void AddHost(AOnlineBeaconHostObject* HostObject);

protected:
	bool IsReady;

};
