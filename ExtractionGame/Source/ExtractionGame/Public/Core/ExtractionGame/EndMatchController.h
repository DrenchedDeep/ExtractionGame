

#pragma once

#include "CoreMinimal.h"
#include "ExtractionGamePlayerState.h"
#include "Extraction/EndGamePlauerStand.h"
#include "GameFramework/Actor.h"
#include "EndMatchController.generated.h"

UENUM(BlueprintType)
enum ELocalPlayerStage
{
	WaitingToStart,
	LookingAtTopPlayers,
	LookingAtStats,
	LoadingMainMenu
};

UCLASS()
class EXTRACTIONGAME_API AEndMatchController : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<AEndGamePlauerStand*> Stands;

    
public:
	void EndMatch(TArray<AExtractionGamePlayerState*> TopThreePlayers);

	UFUNCTION(BlueprintCallable)
	ELocalPlayerStage GetLocalPlayerStage() const { return LocalPlayerStage; }
	UFUNCTION(BlueprintCallable)
	void ContinueToNextStage();
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AEndMatchController();
	
	UFUNCTION()
	void OnRep_MatchEnded();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMatchEnded();
private:
	UPROPERTY(ReplicatedUsing=OnRep_MatchEnded)
	int32 MatchEnded;

	UPROPERTY()
	TEnumAsByte<ELocalPlayerStage> LocalPlayerStage;
};
