

#pragma once

#include "CoreMinimal.h"
#include "GooberManager.h"
#include "GooberTask.h"
#include "Core/CharacterPawnBase.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Actor.h"
#include "Goober.generated.h"


UCLASS()
class EXTRACTIONGAME_API AGoober : public ACharacterPawnBase, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float MaxCookTime = 10.f;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> GooberMeatSubclass;
public:
	UPROPERTY(ReplicatedUsing=OnRep_Dead);
	bool bIsDead;
	UPROPERTY(ReplicatedUsing=OnRep_Cooking);
	bool bIsCooking;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly)
	bool bIsCooked;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void StartCook();
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnRep_Dead();
	UFUNCTION()
	virtual void OnRep_Cooking();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadAll();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadServer();

		
	UFUNCTION(BlueprintImplementableEvent)
	void OnCookTimerEnd();
	UFUNCTION(BlueprintImplementableEvent)
	void OnCookTimerStart();

private:
	float CurrentHealth;
	float CurrentCookTimer;

	UFUNCTION()
	void OnCookTimerTick();

	FTimerHandle CookTimerHandle;
};
