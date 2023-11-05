

#pragma once

#include "CoreMinimal.h"
#include "DeathWidget.h"
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
	UPROPERTY(BlueprintReadWrite)
	UDeathWidget* DeathWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetSubclass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDeathWidget> DeathWidgetSubclass;

	
	UInventoryWidget* CreateInventoryWidget();

private:
	virtual void BeginPlay() override;
};
