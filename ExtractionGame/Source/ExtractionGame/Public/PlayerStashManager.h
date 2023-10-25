

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "PlayerStashManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API APlayerStashManager : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerStashManager();
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStashInventoryComponent* PlayerInventory;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UStashInventoryComponent* StashInventory;

	UFUNCTION(BlueprintCallable)
	 void SaveInventory();
};