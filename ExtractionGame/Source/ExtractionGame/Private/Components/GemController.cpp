


#include "Components/GemController.h"

#include "UI/ExtractionGameHUD.h"
#include "Components/InventoryComponent.h"
#include "UI/Widgets/InventoryWidget.h"
#include "Abilities/GameplayAbility.h"
#include "Components/ItemObject.h"
#include "Components/PlayerInventoryComponent.h"
#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "Core/ExtractionGame/ExtractionGamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/Widgets/PlayerBarDataWidget.h"


AGem** UGemController::GetGemBySlot(EBodyPart slot)
{
	switch (slot) {
	case  Head: return &HeadGem;
	case Chest: return &ChestGem;
	default:
		int val = static_cast<int>(slot) >> 2;
		int index = 0;
		while (val > 1)
		{
			index++;
			val >>= 1;
		}
		if(index < 3) return &leftGems[index];
		return &rightGems[index-3];
	}
}

// Sets default values for this component's properties
UGemController::UGemController(): leftArmCooldown(nullptr), rightArmCooldown(nullptr), SubSystem(nullptr),
                                  LeftAttackAction(nullptr),
                                  RightAttackAction(nullptr),
                                  HeadAbilityAction(nullptr), dirtyFlags(0),
                                  HeadGem(nullptr),
                                  ChestGem(nullptr), Character(nullptr), PlayerBarsWidget(nullptr), bInitialized(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	leftGems.SetNum(3);
	rightGems.SetNum(3);

	// ^^ If timers are fighting me, check here later ^^ 
}


const AGem* UGemController::CheckGem(EBodyPart slot) 
{
	return *GetGemBySlot(slot);
}

void UGemController::AddGem(EBodyPart slot, AGem* newGem)
{
	Server_AddGem(slot, newGem);
}

void UGemController::UpdateAllClientVisuals_Implementation(int flags) const
{
	UE_LOG(LogTemp, Warning, TEXT("Updating visuals? %d"), (GetOwnerRole()))
	//if(Character->GetLocalRole() != ROLE_SimulatedProxy) return;
		
	if((flags & HeadFlag) != 0) OnHeadChanged.Broadcast();
	if((flags & LeftArmFlag) != 0) OnLeftArmChanged.Broadcast();
	if((flags & RightArmFlag) != 0) OnRightArmChanged.Broadcast();
	if((flags & BodyFlag) != 0) OnChestChanged.Broadcast();
}

//Do I need to delete somewhere in here?
void UGemController::RemoveGem(EBodyPart slot)
{
	Server_RemoveGem(slot);
}

void UGemController::Server_CreateGem_Implementation(UItemObject* Item, EBodyPart BodyPart, float purity, bool bAddToInventory)
{
	if(Item)
	{
		CreateGem(Item, BodyPart, 0);

		if(bAddToInventory)
		{
			Character->InventoryComponent->Server_AddGem(Item, BodyPart, purity);
		}
	}
}

void UGemController::CreateGem(const UItemObject* Item, EBodyPart BodyPart, int GemSlotID)
{
	//Is this when a gem is dropped?
	AGem* Gem = GetWorld()->SpawnActor<AGem>();
	UE_LOG(LogTemp, Error, TEXT("REMOVE ME: Pre check Auth: %hhd"), GetOwner()->HasAuthority());
	if(!Item)
	{
		UE_LOG(LogTemp, Error, TEXT("REMOVE ME: Crashed because item is null?"));
		return;
	}
	Gem->SetPolish(Item->DefaultPolish);
	Gem->SetGemType(Item->GemType);
	Client_OnGemCreated(GemSlotID, Gem);

	AGem** gem = GetGemBySlot(BodyPart);
	
	*gem = Gem;
	dirtyFlags |= BodyPart;
	UE_LOG(LogTemp, Warning, TEXT("CreateGem: %d"), BodyPart);
}


void UGemController::Client_OnGemCreated_Implementation(int GemSlotID, AGem* Gem)
{
	Character->InitGemUI();
}

void UGemController::Server_RemoveGem_Implementation(EBodyPart slot)
{
	AGem** gem = GetGemBySlot(slot);
	if(*gem)
	{
		*gem = nullptr;
	}
	dirtyFlags |= slot;
	UE_LOG(LogTemp, Warning, TEXT("RemoveGem: %d"), slot);
}

void UGemController::Server_AddGem_Implementation(EBodyPart slot, AGem* newGem)
{
	AGem** gem = GetGemBySlot(slot);

	// If there was an existing gem, delete it before assigning the new one
	/*
	if (*gem != nullptr)
	{
		delete *gem;
	}*/
	
	*gem = newGem;
	dirtyFlags |= slot;
	UE_LOG(LogTemp, Warning, TEXT("AddGem: %d"), slot);
	//LazyRecompileGems();
} 

void UGemController::OnRep_HeadGem() const
{
	UE_LOG(LogTemp, Warning, TEXT("Head Gem Equipped..."))
	OnHeadChanged.Broadcast(); // We broadcast here too, so when the player is rendered, it loads correctly
	//If this has stopped working, make sure the component is replicated.
	
}

void UGemController::OnRep_ChestGem() const
{
	UE_LOG(LogTemp, Warning, TEXT("Chest Gem Equipped...")) 
	OnChestChanged.Broadcast();
}

void UGemController::OnRep_LeftArmGems() const
{
	UE_LOG(LogTemp, Warning, TEXT("Left Arms Updated... "))
	OnLeftArmChanged.Broadcast();
}

void UGemController::OnRep_RightArmGems() const
{
	UE_LOG(LogTemp, Warning, TEXT("Right Arms Updated... "))
	 OnRightArmChanged.Broadcast();
}

void UGemController::ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const
{
	//If we were previously generating, STOP.
	if(handle->IsValid()) Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(*handle);

	
	FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(Character);

	const FGameplayEffectSpecHandle NewHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(effect, level,EffectContext);
	if (NewHandle.IsValid())
	{
		*handle = Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),  Character->GetAbilitySystemComponent());
	}
}


void UGemController::SmartRecompileGems_Implementation(bool forceRefresh)
{
	
	//if(!GetOwner()->HasAuthority()) return; Not needed, is already a server only function.
	//if(!Character->GetAbilitySystemComponent())
	//{
	//const AExtractionGameCharacter* Ch = Cast<AExtractionGameCharacter>(GetOwner());
	//SubSystem = Ch->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	//Character->GetAbilitySystemComponent() = Ch->Character->GetAbilitySystemComponent();
	//}
//	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP START"))
	
	const int val = forceRefresh?255:dirtyFlags;
	dirtyFlags = None;
	if((val & HeadFlag) != 0)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK HEAD"))

		//HeadAbilityAction = Ch->HeadAbilityAction;
		//HeadEffectHandle.Clear();
		Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(HeadEffectHandle);
		//HeadEffectHandle.Clear();
		//Character->GetAbilitySystemComponent()->ClearAbility(HeadAbilitySpecHandle);
		RecompileHead();
	}
	if((val & BodyFlag) != 0)
	{
//		UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK CHEST"))
	}
	if((val & LeftArmFlag) != 0)
	{
		//LeftAttackAction = Ch->LeftAttackAction;
	//	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK LEFT ARM"))
		Character->GetAbilitySystemComponent()->ClearAbility(LeftArmAbilitySpecHandle);
		RecompileArm(leftGems, true);
	}
	if((val & RightArmFlag) != 0)
	{
		//RightAttackAction = Ch->RightAttackAction;
	//	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK RIGHT ARM"))
		Character->GetAbilitySystemComponent()->ClearAbility(RightArmAbilitySpecHandle);
		RecompileArm(rightGems, false);
	}

	//Lazy.
	hasGem = HeadGem || leftGems[0] || rightGems[0] || leftGems[1] || rightGems[1]; 
	

	UpdateAllClientVisuals(val);
	
	//TODO: Gems affect values.
	//UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP FINAL, %d"), Character->GetLocalRole())
	ApplyEffect(&ManaPoolHandle, ManaPoolEffect, 1);
    ApplyEffect(&ManaRegenHandle, ManaRegenEffect, 1);

}


//void UGemController::InitializeComponent()
//{
//	Super::InitializeComponent();
//}

// Called when the game starts
void UGemController::BeginPlay() // If this isn't working, we init inventory on plater rep srate. do same tihng...
{
	Super::BeginPlay();
	Character = Cast<AExtractionGameCharacter>(GetOwner());
	//if(const AExtractionGetHUDElement()* hud = Cast<AExtractionGetHUDElement()>(Character->GetController<AExtractionGamePlayerController>()->GetHUD()))
	//	GetHUDElement() = hud->PlayerUIData;
	SubSystem = Character->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	//dirtyFlags = 255;
//	UE_LOG(LogTemp, Warning, TEXT("Loading Gem Controller"));
	
	if(Character->GetLocalRole() != ROLE_SimulatedProxy)
	{
		OnLeftManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetLeftManaPoolAttribute()).AddUObject(this, &UGemController::OnLeftManaChanged);
		OnMaxLeftManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxLeftManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxLeftManaChanged);
		OnRightManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetRightManaPoolAttribute()).AddUObject(this, &UGemController::OnRightManaChanged);
		OnMaxRightManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxRightManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxRightManaChanged);
	//	UE_LOG(LogTemp, Warning, TEXT("Controller LOCAL Loaded"))
	}
//	UE_LOG(LogTemp, Warning, TEXT("Controller Loaded"))
	
}

void UGemController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGemController, LeftArmAbilitySpecHandle);
	DOREPLIFETIME(UGemController, RightArmAbilitySpecHandle);
	//DOREPLIFETIME(UGemController, HeadAbilitySpecHandle);
	DOREPLIFETIME(UGemController, HeadGem);
	DOREPLIFETIME(UGemController, ChestGem);
	DOREPLIFETIME(UGemController, leftGems);
	DOREPLIFETIME(UGemController, rightGems);

}

void UGemController::Sadness_Implementation(const FAbilityStruct& abilityInfo, bool isLeft, int ability, float totalPolish)
{
	
	UPlayerBarDataWidget* hud =GetHUDElement();
	//Bro the hud doesn't exist when making the left arm, but not the right arm :(
	UE_LOG(LogTemp, Warning, TEXT("Sadness Called %i is hud? %i"), isLeft, hud != nullptr)
	//BUG when both left and right, only one happens?
	if(isLeft)
	{
		Character->bIsLeftAutomatic = abilityInfo.bIsFullyAuto;
		if(hud) hud->SetLeftGems(leftGems, abilityInfo.Image, ability, totalPolish);
	}
	else
	{
		Character->bIsRightAutomatic = abilityInfo.bIsFullyAuto;

		if(hud) hud->SetRightGems(rightGems, abilityInfo.Image, ability, -totalPolish);
	}
}

//void UGemController::Server_LazyRecompileGems_Implementation()
//{
//	LazyRecompileGems();
//}
void UGemController::RecompileArm(TArray<AGem*> arm,  bool bIsLeft)
{
	float type [] = {0,0,0,0};
	for (const AGem* gem : arm)
	{
		if(!gem) continue;
		switch (gem->GetGemType()) {
		case Earth:
			type [0]+=gem->GetPolish();
			break;
		case Fire:
			type [1]+=gem->GetPolish();
			break;
		case Shadow:
			type [2]+=gem->GetPolish();
			break;
		case Water:
			type [3]+=gem->GetPolish();
			break;
		}
	}

	//Based on some constant number...
	int32 ability = 0;
	int iteration= 0;
	float totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 75) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}

	const FAbilityStruct InAbilityClass = SubSystem->GetAbilityByIndex(ability);

	if(bIsLeft)
	{
		totalPolish = -totalPolish;
		if(totalPolish == 0)
			totalPolish = -1;
	}
	const FGameplayAbilitySpec AbilitySpec(InAbilityClass.GameplayAbilityClass, totalPolish, -1, Character);
	
	if(bIsLeft) LeftArmAbilitySpecHandle = Character->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	else RightArmAbilitySpecHandle = Character->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);

	Sadness(InAbilityClass, bIsLeft, ability, totalPolish);
}

void UGemController::RecompileHead()
{
	//IF we already have an ability, let's remove it.
	if(HeadEffectHandle.IsValid())
	Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(HeadEffectHandle);

//	UE_LOG(LogTemp, Warning, TEXT("Recomp Head in code is head null? %d"), HeadGem == nullptr);
	if(!HeadGem) return;
	const int val = HeadGem->GetPolish();
	int Score = 1;

	//Let's just say everything is level 1. It's easier.
	//if(val >= 150) Score = 3;
	//else if(val >= 75) Score = 2;
	
	Score =  Score << (6-(static_cast<int>(HeadGem->GetGemType())*2));//(Score << (6 - static_cast<int>(HeadGem->GetGemType()) * 2));
	//Left 1,2,4
//	UE_LOG(LogTemp, Warning, TEXT("Head Ability: %d, %d"), Score, val);
	const TSubclassOf<UGameplayEffect> effect = SubSystem->GetEffectByIndex(Score);
	//const FGameplayAbilitySpec EffectSpec(effect, val, -1, Character);
	ApplyEffect(&HeadEffectHandle,effect, val);

	//UE_LOG(LogTemp, Warning, TEXT("Applying effect to self: "))
	
	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetHeadGems(HeadGem);
}

void UGemController::RecompileChest()
{
	if(!ChestGem) return;
//	UE_LOG(LogTemp, Warning, TEXT("Pending Impl"));
}

UPlayerBarDataWidget* UGemController::GetHUDElement()
{
	if(PlayerBarsWidget) return PlayerBarsWidget;
	if(const AExtractionGamePlayerController* x = Cast<AExtractionGamePlayerController>(Character->GetLocalViewingPlayerController()))
	{
		if(const AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(x->GetHUD()))
		{
			PlayerBarsWidget = HUD->GetPlayerBarWidget();
			return PlayerBarsWidget;
		}
	}
	return nullptr;
}

void UGemController::OnLeftManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetLeftManaPercent(Data.NewValue / GetLeftMaxMana());
}

void UGemController::OnMaxLeftManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetLeftManaPercent(GetLeftMana() / Data.NewValue);
}

void UGemController::OnRightManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetRightManaPercent(Data.NewValue / GetRightMaxMana());
}

void UGemController::OnMaxRightManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarDataWidget* hud =GetHUDElement();
	if(!hud) return;
	hud->SetRightManaPercent(GetRightMana() / Data.NewValue);
}

float UGemController::GetLeftMana() const
{
	return Character->GetAttributeSet()->GetLeftManaPool();
}

float UGemController::GetLeftMaxMana() const
{
	return Character->GetAttributeSet()->GetMaxLeftManaPool();
}

float UGemController::GetRightMana() const
{
	return Character->GetAttributeSet()->GetRightManaPool();
}

float UGemController::GetRightMaxMana() const
{
	return Character->GetAttributeSet()->GetMaxRightManaPool();
}

float UGemController::GetManaRegenRate() const
{
	return Character->GetAttributeSet()->GetRegenMana();
}


void UGemController::Initialize(const AExtractionGameHUD* hud)
{
//	UE_LOG(LogTemp, Warning, TEXT("Initializing Gem Controller"))
	if (UPlayerBarDataWidget* PlayerBarWidget = hud->GetPlayerBarWidget())
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Yay"))
		// Use PlayerBarWidget since it's not null
		//GetHUDElement() = PlayerBarWidget;
	}
	//SmartRecompileGems();
}

EBodyPart UGemController::GetNextAvaliableArmGemSlot(bool bIsLeft) const
{
	if(bIsLeft)
	{
		for(int i = 0; i < 3; i++)
		{
			if(leftGems[i] == nullptr)
			{
				return static_cast<EBodyPart>(1 << (i+2));
			}
		}
	}
	else
	{
		for(int i = 0; i < 3; i++)
		{
			if(rightGems[i] == nullptr)
			{
				return static_cast<EBodyPart>(1 << (i+5));
			}
		}
	}
	return None;
}

FAbilityStruct UGemController::GetLeftArmAbilityInfo()
{

	float type [] = {0,0,0,0};
	for(auto Gem : leftGems)
	{
		if(!Gem)
		{
			continue;
		}

		switch(Gem->GetGemType())
		{
		case EGemType::Earth:
			type [0]+=Gem->GetPolish();
			break;
		case EGemType::Fire:
			type [1]+=Gem->GetPolish();
			break;
		case EGemType::Shadow:
			type [2]+=Gem->GetPolish();
			break;
		case EGemType::Water:
			type [3]+=Gem->GetPolish();
			break;
		default:  ;
		}
	}

	int32 ability = 0;
	int iteration= 0;
	float totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 75) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}

	const FAbilityStruct InAbilityClass = SubSystem->GetAbilityByIndex(ability);

	return InAbilityClass;
}

FAbilityStruct UGemController::GetRightArmAbilityInfo()
{
	float type [] = {0,0,0,0};
	for(auto Gem : rightGems)
	{
		if(!Gem)
		{
			continue;
		}

		switch(Gem->GetGemType())
		{
		case EGemType::Earth:
			type [0]+=Gem->GetPolish();
			break;
		case EGemType::Fire:
			type [1]+=Gem->GetPolish();
			break;
		case EGemType::Shadow:
			type [2]+=Gem->GetPolish();
			break;
		case EGemType::Water:
			type [3]+=Gem->GetPolish();
			break;
		default:  ;
		}
	}

	int32 ability = 0;
	int iteration= 0;
	float totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 75) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}

	const FAbilityStruct InAbilityClass = SubSystem->GetAbilityByIndex(ability);

	return InAbilityClass;
}

int32 UGemController::GetArmGemCount(bool bIsLeft) const
{
	int32 count = 0;
	if(bIsLeft)
	{
		for(int i = 0; i < 3; i++)
		{
			if(leftGems[i] != nullptr)
			{
				count++;
			}
		}
	}
	else
	{
		for(int i = 0; i < 3; i++)
		{
			if(rightGems[i] != nullptr)
			{
				count++;
			}
		}
	}
	return count;
}

bool UGemController::HasAnyNumGems()
{
	return hasGem;
}
