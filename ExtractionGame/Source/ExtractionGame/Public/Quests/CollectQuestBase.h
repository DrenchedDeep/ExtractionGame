

#pragma once

#include "CoreMinimal.h"
#include "QuestActor.h"
#include "QuestStatics.h"
#include "CollectQuestBase.generated.h"

USTRUCT()
struct FItemsCollected
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 ItemsNeededIndex;
	UPROPERTY()
	int32 ItemAmount;
};
UCLASS()
class EXTRACTIONGAME_API ACollectQuestBase : public AQuestActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FQuestRewardItem> ItemsNeeded;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FQuestRewardItem> GetItemsNeeded() const { return ItemsNeeded; }
	
protected:
	virtual bool CheckIfQuestIsComplete() override;

private:
	UPROPERTY()
	TArray<FItemsCollected> ItemsCollected;
};
