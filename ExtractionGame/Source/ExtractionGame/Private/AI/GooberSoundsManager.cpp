


#include "AI/GooberSoundsManager.h"

#include "Kismet/GameplayStatics.h"


void AGooberSoundsManager::PlaySFX(FString ID, FVector Location)
{
	Multicast_PlaySFX(ID, Location);
}

void AGooberSoundsManager::Multicast_PlaySFX_Implementation(const FString& ID, FVector Location)
{
	for(int32 i = 0; i < GooberSounds.Num(); i++)
	{
		if(ID == GooberSounds[i].ID)
		{
			TArray<FPercentAudio> audio = GooberSounds[i].Sound; 
			for(int j  = audio.Num()-1; j > 0; --j)
			{
				const float rng = FMath::FRand();
				if(rng <= audio[j].percent) // This is bad probability lol it'd be more optimized if at compile time, we new sum probabilty, took a random in that range, and then just found it's owner by summing.
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(),  audio[j].Sound, Location, FRotator(), Volume, Pitch, 0.f, GooberSoundAttenuation);
					return;
				}
			}
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), audio[0].Sound, Location, FRotator(), Volume, Pitch, 0.f, GooberSoundAttenuation);
			return;
		}
	}
}

AGooberSoundsManager::AGooberSoundsManager()
{
	bReplicates = true;
}

