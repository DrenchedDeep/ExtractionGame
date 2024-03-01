

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

	
	
	FDelegateHandle OnEarthManaChangedHandle;
	FDelegateHandle OnFireManaChangedHandle;
	FDelegateHandle OnWaterManaChangedHandle;
	FDelegateHandle OnShadowManaChangedHandle;

	FDelegateHandle OnMaxEarthManaChangedHandle;
	FDelegateHandle OnMaxFireManaChangedHandle;
	FDelegateHandle OnMaxWaterManaChangedHandle;
	FDelegateHandle OnMaxShadowManaChangedHandle;

	virtual void OnEarthManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxEarthManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnFireManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxFireManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnShadowManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxShadowManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnWaterManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxWaterManaChanged(const FOnAttributeChangeData& Data);


	
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

	int dirtyFlags;

	
public:	
	UGemController();

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle LeftArmAbilitySpecHandle;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle RightArmAbilitySpecHandle;
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Gems Abilites")
	FGameplayAbilitySpecHandle HeadAbilitySpecHandle;

	UPROPERTY(ReplicatedUsing=OnRep_HeadGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* HeadGem;
	
	UPROPERTY(ReplicatedUsing=OnRep_ChestGem, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	AGem* ChestGem;

	UPROPERTY(ReplicatedUsing=OnRep_LeftArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> leftGems;

	UPROPERTY(ReplicatedUsing=OnRep_RightArmGems, VisibleAnywhere, BlueprintReadOnly, Category="Gems")
	TArray<AGem*> rightGems;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> ManaRegenEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASDocumentation|Abilities")
	TSubclassOf<UGameplayEffect> ManaPoolEffect;

	FActiveGameplayEffectHandle ManaRegenHandle;
	FActiveGameplayEffectHandle ManaPoolHandle;

	UPROPERTY(Transient)AExtractionGameCharacter* Character;
	UPROPERTY(Transient)UPlayerBarDataWidget* PlayerBarsWidget;
	
	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. Returns false if slot is already filled"))
	bool CheckGem(EBodyPart slot);
	
	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Add gem into head slot. WARNING: will delete gems if not handled properly"))
	void AddGem(EBodyPart slot, AGem* newGem);

	UFUNCTION(BlueprintCallable, Category="Gems", meta=(ToolTip = "Remove the current head gem. WARNING: Can return null ptrs."))
	void RemoveGem(EBodyPart slot);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_CreateGem(UItemObject* Item, EBodyPart BodyPart, bool bAddToInventory = false);
	UFUNCTION(Client, Reliable)
	void Client_OnGemCreated(int GemSlotID, AGem* Gem);

	void CreateGem(UItemObject* Item, EBodyPart BodyPart, int GemSlotID);
	//UFUNCTION(Server, Reliable)
	//void Server_LazyRecompileGems();
	UFUNCTION(Server, Reliable)
	void Server_AddGem(EBodyPart slot, AGem* newGem);
	UFUNCTION(Server, Reliable)
	void Server_RemoveGem(EBodyPart slot);

	UFUNCTION(Server, Reliable)
	void SmartRecompileGems();

	

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetEarthMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetEarthMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetFireMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetFireMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetShadowMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetShadowMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetWaterMana() const;

	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetWaterMaxMana() const;
	
	UFUNCTION(BlueprintCallable, Category = "Player Attributes")
	float GetManaRegenRate() const;
	void Initialize(const AExtractionGameHUD* hud);

	UFUNCTION(BlueprintCallable)
	EBodyPart GetNextAvaliableArmGemSlot(bool bIsLeft) const;


	UFUNCTION(BlueprintCallable)
	int32 GetArmGemCount(bool bIsLeft) const;
	
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
	void Sadness(bool left, bool state);
	
private:
	void RecompileArm(TArray<AGem*> arm, bool bIsLeft);
	void RecompileHead();
	void RecompileChest();



	UPlayerBarDataWidget* GetHUDElement();


	
	UFUNCTION()
	void OnRep_HeadGem();
	UFUNCTION()
	void OnRep_ChestGem();
	UFUNCTION()
	void OnRep_LeftArmGems();
	UFUNCTION()
	void OnRep_RightArmGems();

	void ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const;

	bool bInitialized;
};
