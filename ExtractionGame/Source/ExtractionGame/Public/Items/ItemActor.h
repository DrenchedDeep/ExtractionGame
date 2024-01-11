

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "Components/SphereComponent.h"
#include "Core/ExtractionGame/Interactable.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS(BlueprintType, Blueprintable)
class EXTRACTIONGAME_API AItemActor : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* SphereComponent;
	
public:
	AItemActor();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString RowName;
	UPROPERTY(BlueprintReadWrite)
	UItemObject* ItemObject;
	UPROPERTY(BlueprintReadWrite, Category = "Gems", meta = (ExposeOnSpawn="true"))
	TEnumAsByte<EGemType> GemType;
	UPROPERTY(BlueprintReadWrite, Category = "Gems", meta = (ExposeOnSpawn="true"))
	float DefaultPolish;
	void PostReplication(TArray<uint8>& Payload);
};
