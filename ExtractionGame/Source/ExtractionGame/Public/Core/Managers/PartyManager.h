

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PartyManager.generated.h"

class APlayerStand;
USTRUCT(BlueprintType)
struct FPartyPlayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	APlayerState* PlayerState;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsHost;

	UPROPERTY()
	bool bIsReady;

	FPartyPlayer(APlayerState* PlayerState, bool bIsHost)
		: PlayerState(PlayerState),
		  bIsHost(bIsHost)
	{
	}

	FPartyPlayer() = default;
};

UCLASS(Blueprintable)
class EXTRACTIONGAME_API APartyManager : public AActor
{
	GENERATED_BODY()
	
public:
	APartyManager();

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PartyPlayers, BlueprintReadOnly)
	TArray<FPartyPlayer> PartyPlayers;

	UPROPERTY(Replicated)
	int32 PartyID;
	
	UFUNCTION()
	void OnRep_PartyPlayers();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayers();
	
	virtual void AddPlayer(APlayerController* PlayerController);
	virtual void RemovePlayer(APlayerController* PlayerController);

	void UpdatePlayerReadyStatus(APlayerState* PlayerState, bool bReady);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FPartyPlayer GetLocalPartyPlayer() const
	{
		FPartyPlayer LocalPlayer = {};
		
		for(int32 i = 0; i < PartyPlayers.Num(); i++)
		{
			if(PartyPlayers[i].PlayerState == UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState)
			{
				LocalPlayer = PartyPlayers[i];
			}
		}

		return LocalPlayer;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FPartyPlayer GetPartyPlayer(APlayerState* PlayerState) const
	{
		FPartyPlayer LocalPlayer = {};
		
		for(int32 i = 0; i < PartyPlayers.Num(); i++)
		{
			if(PartyPlayers[i].PlayerState == PlayerState)
			{
				LocalPlayer = PartyPlayers[i];
			}
		}

		return LocalPlayer;
	}


	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	FORCEINLINE bool AlreadyHasPlayer(const APlayerState* PlayerState)
	{
		bool bHasPlayer = false;
		
		for(int32 i = 0; i < PartyPlayers.Num(); i++)
		{
			bHasPlayer = PartyPlayers[i].PlayerState == PlayerState;
		}

		return bHasPlayer;
	}
};
