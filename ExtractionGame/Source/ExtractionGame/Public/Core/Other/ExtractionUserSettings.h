

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
	UPROPERTY(Config) int AmbientVolume;
	
	
public:

	UFUNCTION(BlueprintCallable)
	static UExtractionUserSettings* GetExtractionUserSettings();

	UFUNCTION(BlueprintCallable)void SetMasterVolume(int newValue) { MasterVolume = newValue; }
	UFUNCTION(BlueprintPure)int GetMasterVolume() const { return MasterVolume; }
	
	UFUNCTION(BlueprintCallable)void SetEffectVolume(int newValue) { EffectVolume = newValue; }
	UFUNCTION(BlueprintPure)int GetEffectVolume() const { return EffectVolume; }
	
	UFUNCTION(BlueprintCallable)void SetMusicVolume(int newValue) { MusicVolume = newValue; }
	UFUNCTION(BlueprintPure)int GetMusicVolume() const { return MusicVolume; }
	
	UFUNCTION(BlueprintCallable)void SetAmbientVolume(int newValue) { AmbientVolume = newValue; }
	UFUNCTION(BlueprintPure)int GetAmbientVolume() const { return AmbientVolume; }
	
};
