

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "NetworkErrorWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UNetworkErrorWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorHeaderText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorDescriptionText;



public:
	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	void OnNetworkError(FString ErrorHeader, FString ErrorDescription);
};
