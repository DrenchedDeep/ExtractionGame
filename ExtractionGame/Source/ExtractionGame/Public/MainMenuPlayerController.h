

#pragma once

#include "CoreMinimal.h"
#include "PlayerStashManager.h"
#include "GameFramework/PlayerController.h"
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

	
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	bool bWantsToHost = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerStashManager* PlayerStashManager;
	
};
