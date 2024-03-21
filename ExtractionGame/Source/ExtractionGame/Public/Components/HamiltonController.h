

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
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsTickingHamilton = false;

	void StartHamiltonProcess();
	void StopHamiltonProcess();

	void OnHamiltonPressed();
	void OnHamiltonReleased();


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
	
private:
	float HamiltonTickTime;
	
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
