

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerInventoryComponent.h"
#include "Components/StashInventoryComponent.h"
#include "GameFramework/Actor.h"
#include "RespawnStashManager.generated.h"

UCLASS()
class EXTRACTIONGAME_API ARespawnStashManager : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* StashInventory;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStashInventoryComponent* PlayerInventory;

	UFUNCTION(BlueprintCallable)
	TMap<TEnumAsByte<EBodyPart>, FAddItemInfo> GetGemInventory();
	
	UFUNCTION(BlueprintCallable)
	void ResetGemInventory();
protected:
	ARespawnStashManager();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddGem(UItemObject* Gem, EBodyPart BodyPart);
	UFUNCTION(BlueprintCallable)
	void RemoveGem(EBodyPart BodyPart);

	UFUNCTION(BlueprintCallable)
	TArray<FGemItem> GetGemInventoryStruct();
	
	UFUNCTION(BlueprintCallable)
	void Save();
	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetLeftArmAbilityInfo();
	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetRightArmAbilityInfo();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateGemUI();
private:
	TMap<TEnumAsByte<EBodyPart>, UItemObject*> GemInventory;

};


	