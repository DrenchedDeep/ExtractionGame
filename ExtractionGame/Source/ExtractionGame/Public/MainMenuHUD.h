

#pragma once

#include "CoreMinimal.h"
#include "AccountLoadingScreenWidget.h"
#include "GameHUD.h"
#include "MainMenuWidget.h"
#include "RaidOverWidget.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class EXTRACTIONGAME_API AMainMenuHUD : public AGameHUD
{
	GENERATED_BODY()

public:
	virtual void PreInitializeComponents() override;

	UPROPERTY(BlueprintReadOnly)
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY(BlueprintReadOnly)
	URaidOverWidget* RaidOverWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetSubclass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URaidOverWidget> RaidOverWidgetSubclass;
};


