

#pragma once

#include "CoreMinimal.h"
#include "Core/Managers/PlayerStashManager.h"
#include "GameFramework/PlayerController.h"
#include "Quests/QuestController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerStashManager> PlayerStashManagerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AQuestController> QuestControllerSubclass;
	
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	bool bWantsToHost = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerStashManager* PlayerStashManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AQuestController* QuestController;
	
};
