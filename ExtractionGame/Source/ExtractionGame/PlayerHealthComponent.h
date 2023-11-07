#pragma once

#include "CoreMinimal.h"
#include "Components/HealthComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerHealthComponent.generated.h"

class AExtractionGameCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UPlayerHealthComponent : public UHealthComponent
{
	GENERATED_BODY()

	UPROPERTY(Transient) AExtractionGameCharacter* Character;

public:	
	UPlayerHealthComponent();

	virtual void InitializeComponent() override;
	virtual void OnDeath(const FString& PlayerName);
	virtual void OnRep_IsDead() override;
	virtual void ApplyDamage(float Damage, const AController* Instigator) override;
	

	UFUNCTION(Client, Unreliable)
	void Client_ApplyDamage();
};
