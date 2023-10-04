

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

	UPROPERTY()
	APlayerState* PlayerState;

	UPROPERTY()
	APlayerStand* PlayerStand;
	
	UPROPERTY()
	bool bIsHost;

	UPROPERTY()
	bool bIsReady;

	FPartyPlayer(APlayerState* PlayerState, APlayerStand* PlayerStand, bool bIsHost)
		: PlayerState(PlayerState),
		  PlayerStand(PlayerStand),
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

	UFUNCTION()
	void OnRep_PartyPlayers();
	
	virtual void AddPlayer(APlayerController* PlayerController, APlayerStand* PlayerStand);
	virtual void RemovePlayer(APlayerController* PlayerController);

	
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
	
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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
