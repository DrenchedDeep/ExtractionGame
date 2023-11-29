


#include "Components/GemController.h"

#include "InventoryComponent.h"
#include "InventoryWidget.h"
#include "Abilities/GameplayAbility.h"
#include "Core/AbilityHandlerSubSystem.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Online/XMPP/Public/XmppMultiUserChat.h"


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
                                  OwnerAbilities(nullptr),
                                  LeftAttackAction(nullptr),
                                  RightAttackAction(nullptr),
                                  HeadAbilityAction(nullptr), dirtyFlags(0),
                                  HeadGem(nullptr),
                                  ChestGem(nullptr), bInitialized(false)
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
	if(AExtractionGameCharacter* Character = Cast<AExtractionGameCharacter>(GetOwner()))
	{
		USlotWidget* Slot = Character->InventoryComponent->InventoryWidget->GetSlot(GemSlotID);

		if(UGemSlot* GemSlot =	Cast<UGemSlot>(Slot))
		{
			GemSlot->Gem = Gem;
			
		}
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

void UGemController::SmartRecompileGems_Implementation()
{
	
	//if(!GetOwner()->HasAuthority()) return; Not needed, is already a server only function.
	//if(!OwnerAbilities)
	//{
	//const AExtractionGameCharacter* Ch = Cast<AExtractionGameCharacter>(GetOwner());
	//SubSystem = Ch->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	//OwnerAbilities = Ch->AbilitySystemComponent;
	//}
	
	const int val = dirtyFlags;
	dirtyFlags = None;
	if((val & HeadFlag) != 0)
	{
		//HeadAbilityAction = Ch->HeadAbilityAction;
		OwnerAbilities->ClearAbility(HeadAbilitySpecHandle);
		RecompileHead();
	}
	if((val & BodyFlag) != 0)
	{
		
	}
	if((val & LeftArmFlag) != 0)
	{
		//LeftAttackAction = Ch->LeftAttackAction;
		OwnerAbilities->ClearAbility(LeftArmAbilitySpecHandle);
		RecompileArm(leftGems, true);
	}
	if((val & RightArmFlag) != 0)
	{
		//RightAttackAction = Ch->RightAttackAction;
		OwnerAbilities->ClearAbility(RightArmAbilitySpecHandle);
		RecompileArm(rightGems, false);
	}
}

void UGemController::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UGemController::BeginPlay()
{
	Super::BeginPlay();

	const AExtractionGameCharacter* Ch = Cast<AExtractionGameCharacter>(GetOwner());
	SubSystem = Ch->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	OwnerAbilities = Ch->AbilitySystemComponent;
	
	if(GetOwner()->HasAuthority())
	{
		dirtyFlags = 255;
		SmartRecompileGems();
	}
}

void UGemController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UGemController, LeftArmAbilitySpecHandle);
	DOREPLIFETIME(UGemController, RightArmAbilitySpecHandle);
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
	
	const FGameplayAbilitySpec AbilitySpec(InAbilityClass, totalPolish, -1, this);

	UE_LOG(LogTemp, Warning, TEXT("Ability: %d"), ability);
	//if(GetOwner()->HasAuthority())
	//{
	if(bIsLeft)
	{
		LeftArmAbilitySpecHandle = OwnerAbilities->GiveAbility(AbilitySpec);
	}
	else
	{
		RightArmAbilitySpecHandle = OwnerAbilities->GiveAbility(AbilitySpec);
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
	const FGameplayAbilitySpec AbilitySpec(InAbilityClass, val, -1, this);
	HeadAbilitySpecHandle = OwnerAbilities->GiveAbility(AbilitySpec);
}

void UGemController::RecompileChest()
{
	if(!ChestGem) return;
	UE_LOG(LogTemp, Warning, TEXT("Pending Impl"));
}



