

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaidOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API URaidOverWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void InitBP(bool bSurvived, float PlayTime);

	UFUNCTION(BlueprintCallable)
	void Next();
	UFUNCTION(BlueprintCallable)
	void Previous();
};
