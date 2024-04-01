

#pragma once

#include "CoreMinimal.h"
#include "GooberTask.h"
#include "Core/CharacterPawnBase.h"
#include "Core/ExtractionGame/Cookable.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Actor.h"
#include "Goober.generated.h"

UENUM(BlueprintType)
enum EGooberStates
{
	EG_Friendly,
	EG_AngryExplode,
	EG_AngryFireball,
	EG_AngryWaterSplash
};

class AGooberSpawnCluster;
UCLASS()
class EXTRACTIONGAME_API AGoober : public ACharacterPawnBase, public IInteractable, public ICookable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MaxCookTime = 10.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> GooberMeatSubclass;
public:
	UPROPERTY(ReplicatedUsing=OnRep_Dead, BlueprintReadOnly);
	bool bIsDead;
	UPROPERTY(ReplicatedUsing=OnRep_Cooking);
	bool bIsCooking;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsCooked;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage);

	void MoveToGeyser(AActor* Geyser);
	
	UFUNCTION(BlueprintCallable)
	void StartCook();
	UFUNCTION(BlueprintCallable)
	void StopCook();
	
	void Init(AGooberSpawnCluster* InGooberSpawnCluster);
	UFUNCTION(BlueprintCallable)
	void SetGooberState(EGooberStates InState) {GooberState = InState;}
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EGooberStates> GetGooberState() const { return GooberState; }
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float CurrentCookTimer = 0;
	
	UFUNCTION(BlueprintCallable)
	void AddCook(float amount);
	
	UFUNCTION()
	virtual void OnRep_Dead();
	UFUNCTION()
	virtual void OnRep_Cooking();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadAll();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadServer();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_MoveToGeyser(AActor* Geyser);
private:
	UPROPERTY()
	AGooberSpawnCluster* GooberSpawn;
	float CurrentHealth;
	
	UFUNCTION()
	void OnCookTimerTick();
	FTimerHandle CookTimerHandle;

	UPROPERTY(Replicated)
	TEnumAsByte<EGooberStates> GooberState;
};
