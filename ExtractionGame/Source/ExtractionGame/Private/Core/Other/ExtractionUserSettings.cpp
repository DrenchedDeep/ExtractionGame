


#include "Core/Other/ExtractionUserSettings.h"

UExtractionUserSettings::UExtractionUserSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	MasterVolume(100),
	EffectVolume(100),
	MusicVolume(20),
	AmbientVolume(20)
{
	//Set defaults.
}

UExtractionUserSettings* UExtractionUserSettings::GetExtractionUserSettings()
{
	return Cast<UExtractionUserSettings>(GetGameUserSettings());
}
