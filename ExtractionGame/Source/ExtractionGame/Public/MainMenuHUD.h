

#pragma once

#include "CoreMinimal.h"
#include "GameHUD.h"
#include "MainMenuWidget.h"
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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetSubclass;
};
