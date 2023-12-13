

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/Item.h"
#include "Engine/DataAsset.h"
#include "ItemDatabase.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UItemDatabase : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UItem*> Items;


	UItem* FindItem(int ItemID)
	{
		for(int i = 0; i < Items.Num(); i++)
		{
			if(ItemID == Items[i]->ItemID)
			{
				return Items[i];
			}
		}
		return nullptr;
	}
	
};
