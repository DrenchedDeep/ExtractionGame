

#pragma once

#include "CoreMinimal.h"
#include "AI/GooberSoundsManager.h"
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
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AGooberSoundsManager* GooberSounds;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGooberSoundsManager> GooberSoundSubclass;


public:
	AExtractionGameState();

//	UFUNCTION(BlueprintCallable, BlueprintPure);
	///AGooberSoundsManager* GetGooberSoundManager() const { return GooberSounds; }

	

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	
};
