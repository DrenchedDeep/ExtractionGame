

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "GameFramework/Actor.h"
#include "QuestActor.generated.h"

UCLASS()
class EXTRACTIONGAME_API AQuestActor : public AActor
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AExtractionGamePlayerController* GetOwningPlayerController() const { return PlayerController; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FName GetRowName() const { return RowName; }
	
	void SetPlayerController(AExtractionGamePlayerController* InPlayerController) { PlayerController = InPlayerController; }

protected:
	virtual void CompleteQuest();
	virtual bool CheckIfQuestIsComplete();
	
private:
	UPROPERTY()
	AExtractionGamePlayerController* PlayerController;
	
	UPROPERTY()
	FName RowName;
};
