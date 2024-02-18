

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "GameFramework/Actor.h"
#include "ExtractionBeacon.generated.h"

UCLASS()
class EXTRACTIONGAME_API AExtractionBeacon : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly,  ReplicatedUsing=OnRep_IsExtracting, Category = "Beacon", meta = (AllowPrivateAccess = "true"))
	bool bIsExtracting;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ExtractionTimer, Category = "Beacon", meta = (AllowPrivateAccess = "true"))
	float ExtractionTimer;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Beacon", meta = (AllowPrivateAccess = "true"))
	bool bPlayerInRange;

	UPROPERTY(EditDefaultsOnly, Category = "Beacon", meta = (AllowPrivateAccess = "true"))
	float ExtractionMaxTimer = 30.f;
	

public:	
	AExtractionBeacon();

	bool GetIsExtracting() const { return bIsExtracting; }
	float GetExtractionTimer() const { return ExtractionTimer; }


	void StartExtraction();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void PlayerEnter(AExtractionGameCharacter* Character);
	UFUNCTION(BlueprintCallable)
	void PlayerExit(AExtractionGameCharacter* Character);
	
	UFUNCTION()
	void OnRep_ExtractionTimer();
	UFUNCTION()
	void OnRep_IsExtracting();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<AExtractionGameCharacter*> Players;

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeaconExtractedServer();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnExtractionStateChangedEverybody(bool bExtracting);
	
private:
	bool bExtractionCompleted;
	void OnExtractionCompleted();

	//if we want just teammates to enter implement it here, but for now anybody can enter one :)
	bool PlayerCanEnterExtractionBeacon(APlayerController* PC);
};
