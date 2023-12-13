

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/NetworkErrorWidget.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNetworkErrorWidget> NetworkErrorWidgetSubclass;

public:
	void CustomNetworkError(FString ErrorReason, FString ErrorDescription);
	void CreateNetworkErrorWidget();

	UPROPERTY()
	UNetworkErrorWidget* NetworkErrorWidget;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSessionFound();
};
