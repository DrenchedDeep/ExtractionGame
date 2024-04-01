

#pragma once

#include "CoreMinimal.h"
#include "ExtractionAbilitySystemComponent.h"
#include "Core/ExtractionGame/Gem.h"
#include "Components/ActorComponent.h"
#include "Core/ExtractionGame/ExtractionGameCharacter.h"
#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "UI/Widgets/PlayerBarDataWidget.h"
#include "GemController.generated.h"

class UItemObject;
class AExtractionGameHUD;
class UItem;

UENUM(BlueprintType)
enum  EBodyPart : uint8
{
	None = 0  UMETA(DisplayName = "NONE"),
	Head = 1    UMETA(DisplayName = "Head"),
	Chest = 2   UMETA(DisplayName = "Chest"),
	LeftArm0 = 4  UMETA(DisplayName = "LeftArm0"),
	LeftArm1 = 8  UMETA(DisplayName = "LeftArm1"),
	LeftArm2 = 16   UMETA(DisplayName = "LeftArm2"),
	RightArm0 = 32 UMETA(DisplayName = "RightArm0"),
	RightArm1 = 64 UMETA(DisplayName = "RightArm1"),
	RightArm2 = 128 UMETA(DisplayName = "RightArm2")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReplicateGemChangeEvent);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXTRACTIONGAME_API UGemController : public UActorComponent 
{
	
	GENERATED_BODY()

	const int HeadFlag = Head;
	const int BodyFlag = Chest;
	const int LeftArmFlag = LeftArm0 | LeftArm1 | LeftArm2;
	const int RightArmFlag = RightArm0 | RightArm1 | RightArm2;
	
	FTimerHandle* leftArmCooldown;
	FTimerHandle* rightArmCooldown;

	UPROPERTY(BlueprintAssignable, Category="Gem Changes")FReplicateGemChangeEvent OnHeadChanged;
	UPROPERTY(BlueprintAssignable, Category="Gem Changes")FReplicateGemChangeEvent OnLeftArmChanged;
	UPROPERTY(BlueprintAssignable, Category="Gem Changes")FReplicateGemChangeEvent OnRightArmChanged;
	UPROPERTY(BlueprintAssignable, Category="Gem Changes")FReplicateGemChangeEvent OnChestChanged;


	FDelegateHandle OnLeftManaChangedHandle;
	FDelegateHandle OnMaxLeftManaChangedHandle;
	FDelegateHandle OnRightManaChangedHandle;
	FDelegateHandle OnMaxRightManaChangedHandle;

	virtual void OnLeftManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxLeftManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnRightManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxRightManaChanged(const FOnAttributeChangeData& Data);

	AGem** GetGemBySlot(EBodyPart slot);

	UPROPERTY()
	UAbilityHandlerSubSystem* SubSystem;
	
	//UPROPERTY()
	//UExtractionAbilitySystemComponent* OwnerAbilities;

	UPROPERTY()
	UInputAction* LeftAttackAction;
	
	UPROPERTY()
	UInputAction* RightAttackAction;
	
	UPROPERTY()
	UInputAction* HeadAbilityAction;

	int dirtyFlags = 255;

	
public:	
	UGemController();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle LeftArmAbilitySpecHandle;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle RightArmAbilitySpecHandle;
	//UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	//FGameplayAbilitySpecHandle HeadAbilitySpecHandle;

	UPROPERTY(ReplicatedUsing=OnRep_HeadGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* HeadGem;
	
	UPROPERTY(ReplicatedUsing=OnRep_ChestGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* ChestGem;

	UPROPERTY(ReplicatedUsing=OnRep_LeftArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> leftGems;

	UPROPERTY(ReplicatedUsing=OnRep_RightArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	//UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> rightGems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> ManaRegenEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> ManaPoolEffect;

	FActiveGameplayEffectHandle ManaRegenHandle;
	FActiveGameplayEffectHandle ManaPoolHandle;
	FActiveGameplayEffectHandle HeadEffectHandle;

	UPROPERTY(Transient)AExtractionGameCharacter* Character;
	UPROPERTY(Transient)UPlayerBarDataWidget* PlayerBarsWidget;
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. Returns false if slot is already filled"))
	const AGem* CheckGem(EBodyPart slot);
	
	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. WARNING: will delete gems if not handled properly"))
	void AddGem(EBodyPart slot, AGem* newGem);
	
	UFUNCTION(NetMulticast, Reliable)// Intended Error, add this. And also, the error liam mentioned could be cus we moved the thing from playerstate to begin play.
	void UpdateAllClientVisuals(int flags) const;

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Remove the current head gem. WARNING: Can return null ptrs."))
	void RemoveGem(EBodyPart slot); 

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CreateGem(UItemObject* Item, EBodyPart BodyPart, float purity, bool bAddToInventory = false);
	UFUNCTION(Client, Reliable)
	void Client_OnGemCreated(int GemSlotID, AGem* Gem);

	void CreateGem(const UItemObject* Item, EBodyPart BodyPart, int GemSlotID);
	//UFUNCTION(Server, Reliable)
	//void Server_LazyRecompileGems();
	UFUNCTION(Server, Reliable)
	void Server_AddGem(EBodyPart slot, AGem* newGem);
	UFUNCTION(Server, Reliable)
	void Server_RemoveGem(EBodyPart slot);

	UFUNCTION(Server, Reliable)
	void SmartRecompileGems(bool forceRefresh = false);

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetLeftMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetLeftMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetRightMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetRightMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetManaRegenRate() const;
	void Initialize(const AExtractionGameHUD* hud);

	UFUNCTION(BlueprintCallable)
	EBodyPart GetNextAvaliableArmGemSlot(bool bIsLeft) const;

	UFUNCTION(BlueprintCallable)
FAbilityStruct GetLeftArmAbilityInfo();
	UFUNCTION(BlueprintCallable)
	FAbilityStruct GetRightArmAbilityInfo();
	
	UFUNCTION(BlueprintCallable)
	int32 GetArmGemCount(bool bIsLeft) const;

	UPROPERTY()
	bool hasGem;
	
	UFUNCTION(BlueprintCallable)
	bool HasAnyNumGems();

protected:
	
	//virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//void LazyRecompileGems();
	//UPROPERTY()
	//UExtractionAbilitySystemComponent* AbilitySystemComponent;
	
	//UPROPERTY()
	//UExtractionAttributeSet* AttributeSetBase;
	UFUNCTION(Client, Reliable)
	void Sadness(const FAbilityStruct& abilityInfo, bool isLeft, int ability, float totalPolish);
	
private:
	void RecompileArm(TArray<AGem*> arm, bool bIsLeft);
	UFUNCTION()
	void RecompileHead();
	void RecompileChest();



	UPlayerBarDataWidget* GetHUDElement();


	
	UFUNCTION()
	void OnRep_HeadGem() const;
	UFUNCTION()
	void OnRep_ChestGem() const;
	UFUNCTION()
	void OnRep_LeftArmGems() const;
	UFUNCTION()
	void OnRep_RightArmGems() const;

	void ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const;

	bool bInitialized;
};
