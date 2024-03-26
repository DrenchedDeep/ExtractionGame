

#pragma once

#include "CoreMinimal.h"
#include "GooberManager.h"
#include "GooberTask.h"
#include "Core/CharacterPawnBase.h"
#include "Core/ExtractionGame/Cookable.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Actor.h"
#include "Goober.generated.h"


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

	UFUNCTION(BlueprintCallable)
	void StartCook();
	UFUNCTION(BlueprintCallable)
	void StopCook();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(Replicated)
	float CurrentCookTimer = 0;
	
	UFUNCTION()
	virtual void OnRep_Dead();
	UFUNCTION()
	virtual void OnRep_Cooking();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadAll();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadServer();
	
private:
	float CurrentHealth;
	
	UFUNCTION()
	void OnCookTimerTick();
	FTimerHandle CookTimerHandle;
};
