

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ExtractionUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class UExtractionUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

protected:
	UPROPERTY(Config) int MasterVolume;
	UPROPERTY(Config) int EffectVolume;
	UPROPERTY(Config) int MusicVolume;
	
	
public:
	UFUNCTION(BlueprintCallable)void SetCustomVariable(FString newValue);
	UFUNCTION(BlueprintPure)FString GetCustomVariable() const;
	
};
