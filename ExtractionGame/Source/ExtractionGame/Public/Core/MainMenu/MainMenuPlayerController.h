

#pragma once

#include "CoreMinimal.h"
#include "ShopManager.h"
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
	TSubclassOf<AShopManager> ShopManagerClass;
	
public:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	bool bWantsToHost = false;

	UPROPERTY(BlueprintReadOnly)
	float ExtractedEssence;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerStashManager* PlayerStashManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AShopManager* ShopManager;
	
};
