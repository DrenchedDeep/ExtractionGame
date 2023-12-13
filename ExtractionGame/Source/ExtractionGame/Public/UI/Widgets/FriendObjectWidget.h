

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FriendObjectWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API UFriendObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FUniqueNetIdRef* UserID;
};
