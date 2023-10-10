


#include "Components/GemController.h"
#include "Abilities/GameplayAbility.h"
#include "Core/AbilityHandlerSubSystem.h"
#include "ExtractionGame/ExtractionGameCharacter.h"


AGem** UGemController::GetGemBySlot(EBodyPart slot)
{
	switch (slot) {
	case EBodyPart::Head:
		return &HeadGem;
	case EBodyPart::Chest:
		return &ChestGem;
	default:
		const int val = static_cast<int>(slot)-2;
		if(val < 3) return &leftGems[val];
		return &rightGems[val-3];
	}
}

// Sets default values for this component's properties
UGemController::UGemController(): leftArmCooldown(nullptr), rightArmCooldown(nullptr), SubSystem(nullptr),
                                  OwnerAbilities(nullptr),
                                  LeftAttackAction(nullptr),
                                  RightAttackAction(nullptr),
                                  HeadAbilityAction(nullptr),
                                  HeadGem(nullptr),
                                  ChestGem(nullptr)
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
	AGem** gem = GetGemBySlot(slot);

	// If there was an existing gem, delete it before assigning the new one
	/*
	if (*gem != nullptr)
	{
		delete *gem;
	}*/
	*gem = newGem;
	LazyRecompileGems();
}
//Do I need to delete somewhere in here?
AGem* UGemController::RemoveGem(EBodyPart slot)
{
	AGem** gem = GetGemBySlot(slot);
	AGem* removedGem = *gem;
	/*
	if(*gem)
	{
		delete *gem;
		*gem = nullptr;
	} */
	LazyRecompileGems();
	return removedGem;
}

// Called when the game starts
void UGemController::BeginPlay()
{
	Super::BeginPlay();
	
	const AExtractionGameCharacter* Ch = Cast<AExtractionGameCharacter>(GetOwner());

	SubSystem = Ch->GetGameInstance()->GetSubsystem<UAbilityHandlerSubSystem>();
	
	OwnerAbilities = Ch->AbilitySystemComponent;
	LeftAttackAction = Ch->LeftAttackAction;
	RightAttackAction = Ch->RightAttackAction;
	HeadAbilityAction = Ch->HeadAbilityAction;

	LazyRecompileGems();
	
}

void UGemController::LazyRecompileGems()
{
	//Change Ability bindings and setups...

	//Bind head ability...
	
	//Bind arm abilities..
	RecompileArm(leftGems, LeftAttackAction);
	RecompileArm(rightGems, RightAttackAction);
	
}

void UGemController::RecompileArm(TArray<AGem*> arm, UInputAction* binding)
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
		if(val >= 150.f) Score = 3;
		else if(val >= 50.f) Score = 2;
		else if (val > 0) Score = 1;
		else Score = 0;
		ability |= Score << (8-(++iteration*2));
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Ability Value: %d"), ability)
	const TSubclassOf<UGameplayAbility> InAbilityClass = SubSystem->GetAbilityByIndex(ability);
	//Hopefully this doesn't allow for multiple.. if it does, just clear before...
	OwnerAbilities->SetInputBinding(binding, OwnerAbilities->GiveAbility(FGameplayAbilitySpec(InAbilityClass, 1, -1, this)));
}



//Probably don't need tick...
/*
void UGemController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/




