

#pragma once

#include "CoreMinimal.h"
#include "GooberTask.h"
#include "GameFramework/Actor.h"
#include "GooberManager.generated.h"

enum EGooberEvents : uint8;
UCLASS()
class EXTRACTIONGAME_API AGooberManager : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGooberTask> GooberSeesPlayer;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGooberTask> GooberSeesSpaceShip;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGooberTask> GooberSeesGoober;
	
public:	
	UGooberTask* GetTaskAtEvent(EGooberEvents InEvent);



};
