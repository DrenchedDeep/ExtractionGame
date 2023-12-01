


#include "Components/GemController.h"

#include "ExtractionGameHUD.h"
#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "Abilities/GameplayAbility.h"
#include "Core/AbilityHandlerSubSystem.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "ExtractionGame/ExtractionGamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "UI/PlayerBarData.h"


AGem** UGemController::GetGemBySlot(EBodyPart slot)
{
	UE_LOG(LogTemp, Warning, TEXT("Find: %d"), slot);
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


bool UGemController::CheckGem(EBodyPart slot)
{
	return *GetGemBySlot(slot) != nullptr;
}

void UGemController::AddGem(EBodyPart slot, AGem* newGem)
{
	Server_AddGem(slot, newGem);
}

//Do I need to delete somewhere in here?
void UGemController::RemoveGem(EBodyPart slot)
{
	Server_RemoveGem(slot);
}

void UGemController::CreateGem(UItem* Item, EBodyPart BodyPart, int GemSlotID)
{
	//Is this when a gem is dropped?
	AGem* Gem = GetWorld()->SpawnActor<AGem>();
	Gem->SetPolish(Item->DefaultPolish);
	Gem->SetGemType(Item->GemType);
	Client_OnGemCreated(GemSlotID, Gem);

	AGem** gem = GetGemBySlot(BodyPart);
	
	*gem = Gem;
	dirtyFlags |= BodyPart;
	UE_LOG(LogTemp, Warning, TEXT("CreateGem: %d"), BodyPart);

	//LazyRecompileGems();
}


void UGemController::Server_CreateGem_Implementation(UItem* Item, EBodyPart BodyPart, int GemSlotID)
{
	CreateGem(Item, BodyPart, GemSlotID);
}


void UGemController::Client_OnGemCreated_Implementation(int GemSlotID, AGem* Gem)
{
	USlotWidget* Slot = Character->InventoryComponent->InventoryWidget->GetSlot(GemSlotID);

	if(UGemSlot* GemSlot =	Cast<UGemSlot>(Slot))
	{
		GemSlot->Gem = Gem;
		
	}
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

void UGemController::OnRep_HeadGem()
{
}

void UGemController::OnRep_ChestGem()
{
}

void UGemController::OnRep_LeftArmGems()
{
}

void UGemController::OnRep_RightArmGems()
{
}

void UGemController::ApplyEffect(FActiveGameplayEffectHandle* handle, TSubclassOf<UGameplayEffect> effect, float level) const
{
	UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK A"))

	
	//If we were previously generating, STOP.
	if(handle->IsValid()) Character->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(*handle);
	
	FGameplayEffectContextHandle EffectContext = Character->GetAbilitySystemComponent()->MakeEffectContext();
	EffectContext.AddSourceObject(Character);

	//TODO: Change level
	UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK B"))
	const FGameplayEffectSpecHandle NewHandle = Character->GetAbilitySystemComponent()->MakeOutgoingSpec(effect, level,EffectContext);
	if (NewHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("APPLY EFFECT CHECK C"))
		*handle = Character->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(),  Character->GetAbilitySystemComponent());
	}
}


void UGemController::SmartRecompileGems_Implementation()
{
	
	//if(!GetOwner()->HasAuthority()) return; Not needed, is already a server only function.
	//if(!Character->GetAbilitySystemComponent())
	//{
	//const AExtractionGameCharacter* Ch = Cast<AExtractionGameCharacter>(GetOwner());
	//SubSystem = Ch->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	//Character->GetAbilitySystemComponent() = Ch->Character->GetAbilitySystemComponent();
	//}
	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP START"))
	const int val = dirtyFlags;
	dirtyFlags = None;
	if((val & HeadFlag) != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK HEAD"))

		//HeadAbilityAction = Ch->HeadAbilityAction;
		Character->GetAbilitySystemComponent()->ClearAbility(HeadAbilitySpecHandle);
		RecompileHead();
	}
	if((val & BodyFlag) != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK CHEST"))
	}
	if((val & LeftArmFlag) != 0)
	{
		//LeftAttackAction = Ch->LeftAttackAction;
		UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK LEFT ARM"))
		Character->GetAbilitySystemComponent()->ClearAbility(LeftArmAbilitySpecHandle);
		RecompileArm(leftGems, true);
	}
	if((val & RightArmFlag) != 0)
	{
		//RightAttackAction = Ch->RightAttackAction;
		UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP CHECK RIGHT ARM"))
		Character->GetAbilitySystemComponent()->ClearAbility(RightArmAbilitySpecHandle);
		RecompileArm(rightGems, false);
	}
	//TODO: Gems affect values.
	UE_LOG(LogTemp, Warning, TEXT("GEM RECOMP FINAL, %d"), Character->GetLocalRole())
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
	dirtyFlags = 255;
	UE_LOG(LogTemp, Warning, TEXT("Loading Gem Controller"));
	
	if(Character->GetLocalRole() == ROLE_AutonomousProxy)
	{
		OnEarthManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetEarthManaPoolAttribute()).AddUObject(this, &UGemController::OnEarthManaChanged);
		OnMaxEarthManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxEarthManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxEarthManaChanged);
		OnFireManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetFireManaPoolAttribute()).AddUObject(this, &UGemController::OnFireManaChanged);
		OnMaxFireManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxFireManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxFireManaChanged);
		OnShadowManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetShadowManaPoolAttribute()).AddUObject(this, &UGemController::OnShadowManaChanged);
		OnMaxShadowManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxShadowManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxShadowManaChanged);
		OnWaterManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetWaterManaPoolAttribute()).AddUObject(this, &UGemController::OnWaterManaChanged);
		OnMaxWaterManaChangedHandle = Character->GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Character->GetAttributeSet()->GetMaxWaterManaPoolAttribute()).AddUObject(this, &UGemController::OnMaxWaterManaChanged);
		UE_LOG(LogTemp, Warning, TEXT("Controller LOCAL Loaded"))
	}
	UE_LOG(LogTemp, Warning, TEXT("Controller Loaded"))
	
}

void UGemController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGemController, LeftArmAbilitySpecHandle);
	DOREPLIFETIME(UGemController, RightArmAbilitySpecHandle);
	DOREPLIFETIME(UGemController, HeadAbilitySpecHandle);
	DOREPLIFETIME(UGemController, HeadGem);
	DOREPLIFETIME(UGemController, ChestGem);
	DOREPLIFETIME(UGemController, leftGems);
	DOREPLIFETIME(UGemController, rightGems);

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
		case EGemType::Earth:
			type [0]+=gem->GetPolish();
			break;
		case EGemType::Fire:
			type [1]+=gem->GetPolish();
			break;
		case EGemType::Shadow:
			type [2]+=gem->GetPolish();
			break;
		case EGemType::Water:
			type [3]+=gem->GetPolish();
			break;
		}
	}

	//Based on some constant number...
	int32 ability = 0;
	//fire, water, light, dark...
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

	const TSubclassOf<UGameplayAbility> InAbilityClass = SubSystem->GetAbilityByIndex(ability);

	if(bIsLeft)
		totalPolish = -totalPolish;
	
	const FGameplayAbilitySpec AbilitySpec(InAbilityClass, totalPolish, -1, Character);

	UE_LOG(LogTemp, Warning, TEXT("Ability: %d"), ability);
	//if(GetOwner()->HasAuthority())
	//{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	if(bIsLeft)
	{
		LeftArmAbilitySpecHandle = Character->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		hud->SetLeftGems(leftGems);
	}
	else
	{
		RightArmAbilitySpecHandle = Character->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
		hud->SetRightGems(rightGems);
	}
	//}
}

void UGemController::RecompileHead()
{
	if(!HeadGem) return;
	const int val = HeadGem->GetPolish();
	int Score;
	
	if(val >= 150) Score = 3;
	else if(val >= 75) Score = 2;
	else return;
	
	Score = (Score << (8-static_cast<int>(HeadGem->GetGemType())*2)) << 8;
	UE_LOG(LogTemp, Warning, TEXT("Ability: %d, %d"), Score, val);
	const TSubclassOf<UGameplayAbility> InAbilityClass = SubSystem->GetAbilityByIndex(Score);
	const FGameplayAbilitySpec AbilitySpec(InAbilityClass, val, -1, Character);
	HeadAbilitySpecHandle = Character->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetHeadGems(HeadGem);
}

void UGemController::RecompileChest()
{
	if(!ChestGem) return;
	UE_LOG(LogTemp, Warning, TEXT("Pending Impl"));
}

UPlayerBarData* UGemController::GetHUDElement()
{
	if(PlayerBarsWidget) return PlayerBarsWidget;
	UE_LOG(LogTemp, Warning, TEXT("Finding UI 1"));
	if(const AExtractionGamePlayerController* x = Cast<AExtractionGamePlayerController>(Character->GetLocalViewingPlayerController()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding UI 2"));
		if(const AExtractionGameHUD* HUD = Cast<AExtractionGameHUD>(x->GetHUD()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Finding UI 3"));
			PlayerBarsWidget = HUD->GetPlayerBarWidget();
		}
	}
	return nullptr;
}


void UGemController::OnEarthManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetEarthManaPercent(Data.NewValue / GetEarthMaxMana());
}

void UGemController::OnMaxEarthManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetEarthManaPercent(GetEarthMana() / Data.NewValue);
}

void UGemController::OnFireManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetFireManaPercent(Data.NewValue / GetFireMaxMana());
}

void UGemController::OnMaxFireManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetFireManaPercent(GetFireMana() / Data.NewValue);
}

void UGemController::OnShadowManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetShadowManaPercent(Data.NewValue / GetShadowMaxMana());
}

void UGemController::OnMaxShadowManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetShadowManaPercent(GetShadowMana() / Data.NewValue);
}

void UGemController::OnWaterManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetWaterManaPercent(Data.NewValue / GetWaterMaxMana());
}

void UGemController::OnMaxWaterManaChanged(const FOnAttributeChangeData& Data)
{
	UPlayerBarData* hud =GetHUDElement();
	if(!hud) return;
	hud->SetWaterManaPercent(GetWaterMana() / Data.NewValue);
}

float UGemController::GetEarthMana() const
{
	
	return Character->GetAttributeSet()->GetEarthManaPool();
}

float UGemController::GetEarthMaxMana() const
{
	return Character->GetAttributeSet()->GetMaxEarthManaPool();
}

float UGemController::GetFireMana() const
{
	return Character->GetAttributeSet()->GetFireManaPool();
}

float UGemController::GetFireMaxMana() const
{
	return Character->GetAttributeSet()->GetMaxFireManaPool();
}

float UGemController::GetShadowMana() const
{
	return Character->GetAttributeSet()->GetShadowManaPool();
}

float UGemController::GetShadowMaxMana() const
{
	return Character->GetAttributeSet()->GetMaxShadowManaPool();
}

float UGemController::GetWaterMana() const
{
	return Character->GetAttributeSet()->GetWaterManaPool();
}

float UGemController::GetWaterMaxMana() const
{
	
	return Character->GetAttributeSet()->GetMaxWaterManaPool();
}

float UGemController::GetManaRegenRate() const
{
	return Character->GetAttributeSet()->GetRegenMana();
}

void UGemController::Initialize(const AExtractionGameHUD* hud)
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing Gem Controller"))
	if (UPlayerBarData* PlayerBarWidget = hud->GetPlayerBarWidget())
	{
		UE_LOG(LogTemp, Warning, TEXT("Yay"))
		// Use PlayerBarWidget since it's not null
		//GetHUDElement() = PlayerBarWidget;
	}

	//SmartRecompileGems();
}
