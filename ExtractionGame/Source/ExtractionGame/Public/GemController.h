

#pragma once

#include "CoreMinimal.h"
#include "Gem.h"
#include "Components/ActorComponent.h"
#include "GemController.generated.h"

DECLARE_DELEGATE(FAttackSignature)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UGemController : public UActorComponent
{
	GENERATED_BODY()

	void HandleArm(UGem* a, UGem* b, UGem* chest, FAttackSignature** delegate) const;

	FAttackSignature* LeftAttackFunc;
	FAttackSignature* RightAttackFunc;

	FTimerHandle* leftArmCooldown;
	FTimerHandle* rightArmCooldown;
	

public:	
	// Sets default values for this component's properties
	UGemController();

	UPROPERTY(BlueprintReadWrite)
	UGem* Mind;
	UPROPERTY(BlueprintReadWrite)
	UGem* Body;
	UPROPERTY(BlueprintReadWrite)
	UGem* LeftOne;
	UPROPERTY(BlueprintReadWrite)
	UGem* LeftTwo;
	UPROPERTY(BlueprintReadWrite)
	UGem* RightOne;
	UPROPERTY(BlueprintReadWrite)
	UGem* RightTwo;

	
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RecompileGems();
	void LeftAttack();
	void RightAttack();
	
	
		
};
