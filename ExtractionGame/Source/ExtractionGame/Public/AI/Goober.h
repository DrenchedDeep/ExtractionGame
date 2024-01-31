

#pragma once

#include "CoreMinimal.h"
#include "GooberManager.h"
#include "GooberTask.h"
#include "Core/CharacterPawnBase.h"
#include "GameFramework/Actor.h"
#include "Goober.generated.h"


UCLASS()
class EXTRACTIONGAME_API AGoober : public ACharacterPawnBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing=OnRep_Dead);
	bool bIsDead;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxHealth;

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(float Damage);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnRep_Dead();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadAll();
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnGooberDeadServer();

private:
	float CurrentHealth;
};
