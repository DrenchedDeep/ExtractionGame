

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	IT_Instant UMETA(DisplayName = "Instant"),
	IT_Hold UMETA(DisplayName = "Hold"),
	IT_None UMETA(DisplayName = "None")
};

UCLASS()
class EXTRACTIONGAME_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TEnumAsByte<EInteractionType> InteractType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShowUIOnLook = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText InteractablePrompt = FText::FromString("Interact");
};
