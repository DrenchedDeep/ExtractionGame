

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EXTRACTIONGAME_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnFriendObject(const FString& Username);

	UFUNCTION()
	void OnFriendsReadComplete(bool bWasSuccess);
	
	virtual void BeginPlay() override;
};
