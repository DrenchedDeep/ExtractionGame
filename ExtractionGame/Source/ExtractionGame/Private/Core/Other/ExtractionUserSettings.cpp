


#include "Core/Other/ExtractionUserSettings.h"

UExtractionUserSettings::UExtractionUserSettings(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer),
	MasterVolume(50),
	EffectVolume(50),
	MusicVolume(50),
	AmbientVolume(20),
	VoiceVolume(100)
{
	//Set defaults.
}

UExtractionUserSettings* UExtractionUserSettings::GetExtractionUserSettings()
{
	return Cast<UExtractionUserSettings>(GetGameUserSettings());
}
