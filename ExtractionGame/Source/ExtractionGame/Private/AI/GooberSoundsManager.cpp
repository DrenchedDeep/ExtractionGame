


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
			int32 RandomSFX = FMath::RandRange(0, GooberSounds[i].Sound.Num());

			if(!GooberSounds[i].Sound.IsValidIndex(RandomSFX))
			{
				RandomSFX = 0;
			}
			
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GooberSounds[i].Sound[RandomSFX], Location, FRotator(), Volume, Pitch, 0.f, GooberSoundAttenuation);
		}
	}
}

AGooberSoundsManager::AGooberSoundsManager()
{
	bReplicates = true;
}

