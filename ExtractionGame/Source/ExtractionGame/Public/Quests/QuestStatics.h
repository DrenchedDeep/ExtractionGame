

#pragma once

#include "CoreMinimal.h"
#include "QuestActor.h"
#include "QuestStatics.generated.h"

UENUM(Blueprintable, BlueprintType)
enum EQuestDifficulty
{
	EQ_Easy UMETA(DisplayName = "Easy"),
	EQ_Difficult UMETA(DisplayName = "Difficult"),
	EQ_Impossible UMETA(DisplayName = "Impossible")
};

USTRUCT(BlueprintType, Blueprintable)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ObjectiveInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AQuestActor> QuestActorClass;
};

USTRUCT(BlueprintType, Blueprintable)
struct FQuestRewardItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemAmount;
};

USTRUCT(BlueprintType, Blueprintable)
struct FQuestReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestRewardItem> RewardItems;
};




USTRUCT(BlueprintType, Blueprintable)
struct FQuestInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EQuestDifficulty> QuestDifficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestObjective> QuestObjectives;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FQuestReward> QuestReward;
};


UCLASS()
class EXTRACTIONGAME_API AQuestStatics : public AActor
{

	GENERATED_BODY()
};
