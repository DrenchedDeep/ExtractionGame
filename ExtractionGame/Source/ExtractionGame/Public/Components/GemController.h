

#pragma once

#include "CoreMinimal.h"
#include "Gem.h"
#include "Attack.h"
#include "Components/ActorComponent.h"
#include "GemController.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UGemController : public UActorComponent
{
	GENERATED_BODY()
	
	FTimerHandle* leftArmCooldown;
	FTimerHandle* rightArmCooldown;
	

public:	
	// Sets default values for this component's properties
	UGemController();

	UPROPERTY(BlueprintReadWrite)
	AGem* Mind;
	UPROPERTY(BlueprintReadWrite)
	AGem* Body;

	//UPROPERTY(BlueprintReadWrite)
	//UAttack* leftArm;

	//UPROPERTY(BlueprintReadWrite)
	//UAttack* rightArm;
	
	

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
