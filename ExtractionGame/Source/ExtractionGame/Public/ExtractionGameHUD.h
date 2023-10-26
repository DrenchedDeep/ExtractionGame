

#pragma once

#include "CoreMinimal.h"
#include "GameHUD.h"
#include "InventoryWidget.h"
#include "GameFramework/HUD.h"
#include "ExtractionGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AExtractionGameHUD : public AGameHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	UInventoryWidget* InventoryWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetSubclass;


	UInventoryWidget* CreateInventoryWidget();
	
};
