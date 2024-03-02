

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Cookable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCookable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXTRACTIONGAME_API ICookable 
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Cooking")
	void OnStartCook();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Cooking")
	void OnStopCook();
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Cooking")
	void OnCookTick(float currentPercent);

};
