

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "GooberSoundsManager.generated.h"

USTRUCT()
struct FGooberSFX
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ID;
	UPROPERTY(EditAnywhere)
	TArray<USoundWave*> Sound;
};
UCLASS()
class EXTRACTIONGAME_API AGooberSoundsManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FGooberSFX> GooberSounds;

	UPROPERTY(EditDefaultsOnly)
	USoundAttenuation* GooberSoundAttenuation;

	UPROPERTY(EditDefaultsOnly)
	float Volume = 1.f;
	UPROPERTY(EditDefaultsOnly)
	float Pitch = 1.f;
	
public:
	UFUNCTION(BlueprintCallable)
	void PlaySFX(FString ID, FVector Location);

protected:
	AGooberSoundsManager();

private:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlaySFX(const FString& ID, FVector Location);
};