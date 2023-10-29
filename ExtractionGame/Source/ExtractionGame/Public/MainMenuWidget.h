

#pragma once

#include "CoreMinimal.h"
#include "RaidOverWidget.h"
#include "StashInventoryWidget.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UStashInventoryWidget* StashInventoryWidget;
};
