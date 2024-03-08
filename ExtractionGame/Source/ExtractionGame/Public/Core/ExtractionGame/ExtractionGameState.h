

#pragma once

#include "CoreMinimal.h"
#include "AI/GooberSoundsManager.h"
#include "GameFramework/GameStateBase.h"
#include "Managers/ItemReplicationManager.h"
#include "ExtractionGameGameMode.h"
#include "ExtractionGameState.generated.h"

USTRUCT(BlueprintType)
struct FReplicatedPartyInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<APlayerState*> PartyMembers;
	UPROPERTY(BlueprintReadOnly)
	int32 PartyID;
};

UCLASS()
class EXTRACTIONGAME_API AExtractionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGooberSoundsManager* GooberSounds;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGooberSoundsManager> GooberSoundSubclass;

public:
	AExtractionGameState();
	
	void OnPlayerKilled(const FString& KillerName, const FString& VictimName, const FString& DeathCause);

	UFUNCTION(BlueprintCallable)
	TArray<FReplicatedPartyInfo> GetReplicatedParties() const { return ReplicatedParties; }
	
	UFUNCTION(BlueprintCallable)
	FReplicatedPartyInfo GetPartyByID(int32 PartyID);
	
	void UpdateParties(TArray<FInGameParty> Parties);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;


private:
	UPROPERTY(Replicated)
	TArray<FReplicatedPartyInfo> ReplicatedParties;
};
