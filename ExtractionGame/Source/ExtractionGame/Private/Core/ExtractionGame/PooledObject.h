

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPooledObjectDespawn, APooledObject*, poolActor, FString, PoolName);

UCLASS(Abstract, Blueprintable)
class EXTRACTIONGAME_API APooledObject : public AActor
{
	GENERATED_BODY()
	FTimerHandle timer;
public:	
	// Sets default values for this actor's properties
	APooledObject();

	//FOnPooledObjectDespawn OnPooledObjectDespawn;
	
	UFUNCTION(BlueprintCallable, Category="Pooled Object")
	void SetActiveState(bool isOn);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Pooling")
	void SetAttributes(float power, FName groupName);

	UPROPERTY(BlueprintReadWrite)
	AExtractionGameCharacter* AbilityOwner;

	
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Pooled Object")
	void SetActiveCustom(bool isOn);

	virtual void SetLifeSpan(float InLifespan) override;
	
	UFUNCTION(BlueprintCallable, Category="Pooled Object")
	void ReturnToPool();

	UPROPERTY(BlueprintReadWrite, Replicated)
	float Power;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
};

