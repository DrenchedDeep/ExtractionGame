


#include "Attack.h"

Attack::Attack()
{
}

void Attack::Invoke() const
{
	//Erm no error handling on this one?
	UE_LOG(LogTemp, Warning, TEXT("Invoked!"));
	//&ptr(totalPolish);
}

void Attack::Recompile()
{
	float type [] = {0,0,0,0};
	for (const AGem* gem : Gems)
	{
		switch (gem->GetGemType()) {
		case EGemType::Fire:
			type [0]+=gem->GetPolish();
			break;
		case EGemType::Water:
			type [1]+=gem->GetPolish();
			break;
		case EGemType::Earth:
			type [2]+=gem->GetPolish();
			break;
		case EGemType::Dark:
			type [3]+=gem->GetPolish();
			break;
		}
	}

	//Based on some constant number...
	int ability = 0;

	//fire, water, light, dark...
	int iteration= 0;
	totalPolish = 0;
	for (const float val : type)
	{
		totalPolish += val;
		int Score;
		if(val >= 150) Score = 3;
		else if(val >= 50) Score = 2;
		else Score = 1;
		ability |= Score << (iteration++*2);
	}

	UE_LOG(LogTemp, Warning, TEXT("Creating ability with score: %d"), ability);
	
//	ptr = UGemAbilities::FindAbility(ability);
}

AGem* Attack::RemoveGem(int id)
{
	AGem* gem = Gems[id];
	Gems[id] = nullptr;
	Recompile();
	return gem;
}

void Attack::AddGem(AGem* gem, int slot)
{
	Gems[slot] = gem;
	Recompile();
}


Attack::~Attack()
{
}
