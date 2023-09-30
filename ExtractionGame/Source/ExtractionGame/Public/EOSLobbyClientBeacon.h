

#pragma once

#include "CoreMinimal.h"
#include "OnlineBeaconClient.h"
#include "EOSLobbyClientBeacon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeaconPingComplete, int32, TimeMS);

DECLARE_LOG_CATEGORY_EXTERN(FBeaconLog, Log, All);

UCLASS()
class EXTRACTIONGAME_API AEOSLobbyClientBeacon : public AOnlineBeaconClient
{
	GENERATED_BODY()

	virtual void OnFailure() override;

public:
	AEOSLobbyClientBeacon(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
bool StartBeacon(FString Address, int32 Port, const bool PortOverride = false);
	UFUNCTION(BlueprintCallable)
	void DisconnectBeacon();

	
	UPROPERTY(BlueprintAssignable)
	FOnBeaconPingComplete OnPingComplete;

protected:
	int64 StartTime;
};
