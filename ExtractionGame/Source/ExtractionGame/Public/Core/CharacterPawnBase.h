


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterPawnBase.generated.h"

UENUM(BlueprintType)
enum  EPawnType : uint8
{
	Player,
	Goober
};


UCLASS()
class EXTRACTIONGAME_API ACharacterPawnBase : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EPawnType> PawnType;
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void OnSeenPawn(APawn* InPawn);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TEnumAsByte<EPawnType> GetPawnType() const { return PawnType; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnSeenPawn(APawn* InPawnObserver);
};
