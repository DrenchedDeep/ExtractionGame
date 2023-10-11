

#pragma once

#include "CoreMinimal.h"
#include "ExtractionAbilitySystemComponent.h"
#include "Gem.h"
#include "Components/ActorComponent.h"
#include "GemController.generated.h"

class UAbilityHandlerSubSystem;

UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	Head = 0    UMETA(DisplayName = "Head"),
	Chest = 1   UMETA(DisplayName = "Chest"),
	LeftArm0 = 2  UMETA(DisplayName = "LeftArm0"),
	LeftArm1 = 3  UMETA(DisplayName = "LeftArm1"),
	LeftArm2 = 4   UMETA(DisplayName = "LeftArm2"),
	RightArm0 = 5 UMETA(DisplayName = "RightArm0"),
	RightArm1 = 6 UMETA(DisplayName = "RightArm1"),
	RightArm2 = 7 UMETA(DisplayName = "RightArm2")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UGemController : public UActorComponent
{
	GENERATED_BODY()
	
	FTimerHandle* leftArmCooldown;
	FTimerHandle* rightArmCooldown;
	
	AGem** GetGemBySlot(EBodyPart slot);

	UPROPERTY()
	UAbilityHandlerSubSystem* SubSystem;
	
	UPROPERTY()
	UExtractionAbilitySystemComponent* OwnerAbilities;

	UPROPERTY()
	UInputAction* LeftAttackAction;
	
	UPROPERTY()
	UInputAction* RightAttackAction;
	
	UPROPERTY()
	UInputAction* HeadAbilityAction;

	
public:	
	// Sets default values for this component's properties
	UGemController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* HeadGem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* ChestGem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> leftGems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> rightGems;

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. Returns false if slot is already filled"))
	bool CheckGem(EBodyPart slot);
	
	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. WARNING: will delete gems if not handled properly"))
	void AddGem(EBodyPart slot, AGem* newGem);

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Remove the current head gem. WARNING: Can return null ptrs."))
	AGem* RemoveGem(EBodyPart slot);

protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void LazyRecompileGems();

	void RecompileArm(TArray<AGem*> arm, UInputAction* binding);
	
};
