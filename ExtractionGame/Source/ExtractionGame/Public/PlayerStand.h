

#pragma once

#include "CoreMinimal.h"
#include "CharacterModel.h"
#include "GameFramework/Actor.h"
#include "PlayerStand.generated.h"

UCLASS(Blueprintable, BlueprintType)
class EXTRACTIONGAME_API APlayerStand : public AActor
{
	GENERATED_BODY()


public:	
	APlayerStand(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY()
	AController* OwningClient;
	
	UPROPERTY(ReplicatedUsing=OnRep_IsOccupied, VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccupied;

	UFUNCTION()
	void OnRep_IsOccupied();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCharacterVisibility(bool bVisible);
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
