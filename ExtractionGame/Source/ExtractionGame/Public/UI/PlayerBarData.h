

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerBarData.generated.h"

/**
 *  This needs to be it's own class so we can put load onto GemPlayerState.
 */
UCLASS()
class EXTRACTIONGAME_API UPlayerBarData : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercent(float percent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetEarthManaPercent(float percent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFireManaPercent(float percent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetShadowManaPercent(float percent);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetWaterManaPercent(float percent);
};
