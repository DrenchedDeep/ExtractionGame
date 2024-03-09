

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/Gem.h"
#include "Blueprint/UserWidget.h"
#include "PlayerBarDataWidget.generated.h"

/**
 *  This needs to be it's own class so we can put load onto GemPlayerState.
 */
UCLASS()
class EXTRACTIONGAME_API UPlayerBarDataWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLeftManaPercent(float percent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetRightManaPercent(float percent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercent(float percent);
	

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLeftGems(const  TArray<AGem*>& Gems, const UTexture2D* AbilityImage, int abilityID, float polish);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) // This would be a client function if we even still want it.
	void SetRightGems(const TArray<AGem*>& Gems, const UTexture2D* AbilityImage, int abilityID, float polish); 

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHeadGems(const AGem* Gems);
	
};
