

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
	
	UPROPERTY(BlueprintReadOnly)
	APlayerState* OwningClient;

	UPROPERTY(ReplicatedUsing=OnRep_IsOccupied, VisibleAnywhere, BlueprintReadOnly)
	bool bIsOccupied;
	UPROPERTY(ReplicatedUsing=OnRep_IsOccupied, VisibleAnywhere, BlueprintReadOnly)
	FString Username;
	
	UFUNCTION()
	void OnRep_IsOccupied();
	UFUNCTION()
	void OnRep_Username();

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleCharacterVisibility(bool bVisible);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUsernameText(const FString& PlayerName);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
