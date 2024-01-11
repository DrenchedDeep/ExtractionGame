

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/SlotWidget.h"
#include "Components/GemController.h"
#include "GemSlot.generated.h"


/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UGemSlot : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* SLotIconImage;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<EBodyPart> BodyPart;
	UPROPERTY(BlueprintReadOnly)
	UItemObject* ItemObject;
	UPROPERTY(BlueprintReadWrite)
	UInventoryComp* InventoryComp;

	
	UFUNCTION(BlueprintCallable)
	void InitNewGem(UItemObject* ItemObj);
	UFUNCTION(BlueprintCallable)
	void ResetGemSlot();
};	