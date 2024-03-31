

#pragma once

#include "CoreMinimal.h"
#include "HamiltonBuildOutline.h"
#include "Components/ActorComponent.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "HamiltonController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UHamiltonController : public UActorComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> HamiltonActorClass;
	UPROPERTY(EditAnywhere)
	float HamiltonMaxTime = 2.f;
	UPROPERTY(EditAnywhere)
	float HamiltonSpawnTime = 0.5f;
	UPROPERTY(EditAnywhere)
	FVector SpawnOffsetFromPlayer;

	UPROPERTY(EditAnywhere)
	float MinEssenceThreshold;
	UPROPERTY()bool HamiltonBuilt;

public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsTickingHamilton = false;

	void StartHamiltonProcess();
	void StopHamiltonProcess();

	void OnHamiltonPressed();
	void OnHamiltonReleased();

	UFUNCTION(BlueprintPure, BlueprintCallable)
	float GetMinEssenceThreshold() const {return MinEssenceThreshold;} 
	

	UFUNCTION(BlueprintPure)
	float GetHamiltonProcessTime() const { return HamiltonTickTime; }
	UFUNCTION(BlueprintPure)
	float GetHamiltonProcessTimePercent() const { return HamiltonTickTime / HamiltonMaxTime; }

	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Server, Reliable)
	void Server_SpawnHamilton(FVector Location);
	UFUNCTION(Server, Reliable)
	void Server_HamiltonStarted();
	UFUNCTION(Server, Unreliable)
	void Server_CancelHamilton();
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_UpdateHamilton(bool Cancelled);
	UFUNCTION(Client, Reliable)
	void Client_SetHamilton(AActor* InActor);
	
	
private:
	float HamiltonTickTime;
	bool bSentRPC;
	UPROPERTY()
	AActor* HamiltonActor;
	UPROPERTY()
	FVector LastSpawnLocation;
	UPROPERTY()
	AExtractionGameCharacter* Character;
	
	FTimerHandle HamiltonBuildTimerHandle;
	UFUNCTION()
	void TickHamiltonBuild();

	bool IsLocationBuildable();
};
