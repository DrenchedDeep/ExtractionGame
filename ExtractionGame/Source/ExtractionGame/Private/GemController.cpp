


#include "GemController.h"

#include "GemAbilities.h"


// Sets default values for this component's properties
UGemController::UGemController(): Mind(), Body(), LeftOne(nullptr), LeftTwo(nullptr), RightOne(nullptr),
                                  RightTwo(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGemController::BeginPlay()
{
	Super::BeginPlay();

	RecompileGems();
	
}
//Honestly a bit lazy...
void UGemController::HandleArm(UGem* a, UGem* b, UGem* chest, FAttackSignature** delegate) const
{
	float TotalPolish = 0;
	const EGemType ChestType = chest->GetGemType();
	const float ChestPolish = chest->GetPolish()/2;
	const int GemCombo = (static_cast<int>(ChestType) | static_cast<int>(a->GetGemType()) | static_cast<int>(b->GetGemType()));
	
	//It's written like this in case a we add forloops down the line
	TotalPolish += a->GetPolish() + (a->GetGemType() == ChestType)?ChestPolish:ChestPolish/2;
	TotalPolish += b->GetPolish() + (b->GetGemType() == ChestType)?ChestPolish:ChestPolish/2; // div 4 for non matching types...

	
	//Select attack based on all types combines.
	FAttackSignature attack = UGemAbilities::FindAbility(GemCombo, TotalPolish);

	
	
	(*delegate)->BindLambda([TotalPolish, GemCombo, attack]
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing with type(s): %d, and total polish of: %f"), GemCombo, TotalPolish);
		attack.Execute();
		//We don't want to be finding out what the ability is here every time...
	});
	
}


void UGemController::RecompileGems()
{
	//Build Right arm...
	
	
}

void UGemController::LeftAttack()
{
	LeftAttackFunc->Execute();
}

void UGemController::RightAttack()
{
	RightAttackFunc->Execute();
}


//Probably don't need tick...
/*
void UGemController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/




