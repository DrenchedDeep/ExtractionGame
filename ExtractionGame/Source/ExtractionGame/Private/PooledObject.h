

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPooledObjectDespawn, APooledObject*, poolActor, FString, PoolName);

UCLASS(Abstract, Blueprintable)
class EXTRACTIONGAME_API APooledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APooledObject();

	FOnPooledObjectDespawn OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable, Category="Pooled Object")
	void SetActivate(bool isOn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Pooling")
	void SetAttributes(float power, FName groupName);
	//virtual void SetAttributes_Implementation(float power, FName groupName);


	
	
protected:
	virtual void Destroyed() override;

	UPROPERTY(BlueprintReadWrite)
	float Power;
};

