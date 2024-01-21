

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Managers/ItemReplicationManager.h"
#include "ExtractionGameState.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGameState : public AGameStateBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UItemReplicationManager* ItemReplicationManager;


public:
	AExtractionGameState();

	UItemReplicationManager* GetItemReplicationManager() const { return ItemReplicationManager; }

	

protected:
	virtual void BeginPlay() override;

	
};
