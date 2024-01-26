

#pragma once

#include "CoreMinimal.h"
#include "QuestActor.h"
#include "EliminateQuestBase.generated.h"

UENUM(BlueprintType)
enum EEliminateTarget
{
	ET_Player UMETA(DisplayName = "Player"),
	ET_NPC UMETA(DisplayName = "NPC"),
	ET_Any UMETA(DisplayName = "Any")
};
UCLASS()
class EXTRACTIONGAME_API AEliminateQuestBase : public AQuestActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<EEliminateTarget> EliminateTarget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 EliminateAmount;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EEliminateTarget> GetEliminateTarget() const { return EliminateTarget; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetEliminateAmount() const { return EliminateAmount; }

	
protected:
	virtual bool CheckIfQuestIsComplete() override;
private:
	UPROPERTY()
	int32 EliminateCount;
};
