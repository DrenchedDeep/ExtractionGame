

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "SlotWidget.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UImage* SlotIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* SlotStackText;
	
public:

	UPROPERTY(BlueprintReadOnly)
	bool bIsOccupied;
	
	void Init(int ID);
	void InitItem(UItem* Item, int StackSize);

	UFUNCTION(BlueprintCallable)
	int GetSlotID() const { return SlotID;}
	
private:
	int SlotID;
};
