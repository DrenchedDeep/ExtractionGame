

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPooledObjectDespawn, APooledObject*, poolActor, FString, PoolName);

UCLASS()
class EXTRACTIONGAME_API APooledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledObject();

	FOnPooledObjectDespawn OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable, Category="Pooled Object")
	void SetActivate(bool isOn);
	
protected:
	virtual void Destroyed() override;
};
