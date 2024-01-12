

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemObject.h"
#include "ReplicatedItemObject.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UReplicatedItemObject : public UItemObject
{
	GENERATED_BODY()

public:
	
	//make sure our item has a valid uworld
	virtual UWorld* GetWorld() const override
	{
		if(const UObject* Outer = GetOuter())
		{
			return Outer->GetWorld();
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwningActor() const { return GetTypedOuter<AActor>();}

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void Destroy();
	
	virtual bool CallRemoteFunction(UFunction* Function, void* Parameters, struct FOutParmRec* OutParams, FFrame* Stack) override;
	
protected:
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void OnDestroyed();
};



