

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
	UGemController();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle LeftArmAbilitySpecHandle;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle RightArmAbilitySpecHandle;

	UPROPERTY(ReplicatedUsing=OnRep_HeadGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* HeadGem;
	
	UPROPERTY(ReplicatedUsing=OnRep_ChestGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* ChestGem;

	UPROPERTY(ReplicatedUsing=OnRep_LeftArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> leftGems;

	UPROPERTY(ReplicatedUsing=OnRep_RightArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> rightGems;

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. Returns false if slot is already filled"))
	bool CheckGem(EBodyPart slot);
	
	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. WARNING: will delete gems if not handled properly"))
	void AddGem(EBodyPart slot, AGem* newGem);

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Remove the current head gem. WARNING: Can return null ptrs."))
	AGem* RemoveGem(EBodyPart slot);

	UFUNCTION()
	void OnRep_HeadGem();
	UFUNCTION()
	void OnRep_ChestGem();
	UFUNCTION()
	void OnRep_LeftArmGems();
	UFUNCTION()
	void OnRep_RightArmGems();
	
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	void LazyRecompileGems();

	void Attack(bool bLeftArm);
	void RecompileArm(TArray<AGem*> arm, bool bIsLeft);

private:
	UFUNCTION(Server, Reliable)
	void Server_LazyRecompileGems();
	UFUNCTION(Server, Reliable)
	void Server_AddGem(EBodyPart slot, AGem* newGem);
	UFUNCTION(Server, Reliable)
	void Server_RemoveGem(EBodyPart slot);
	
};
