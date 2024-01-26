

#pragma once

#include "CoreMinimal.h"
#include "QuestStatics.h"
#include "GameFramework/Actor.h"
#include "QuestController.generated.h"

UCLASS()
class EXTRACTIONGAME_API AQuestController : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UDataTable* QuestDataTable;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FQuestInfo> GetPlayerQuests() const { return PlayerQuests; }

	virtual void InitQuests();
private:
	UPROPERTY()
	TArray<FQuestInfo> PlayerQuests;
};
