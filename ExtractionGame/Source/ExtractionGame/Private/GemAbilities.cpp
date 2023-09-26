


#include "GemAbilities.h"

//It'd be faster to pre-convert it manually, but it'd be so minute im not sure it's worth it...
int UGemAbilities::convertToInteger(int fire, int water, int light, int dark)
{
	return (fire << 6) | (water << 4) | (light << 2) | dark;
}

void UGemAbilities::FireBolt(float pow)
{
	UE_LOG(LogTemp, Warning, TEXT("FireBolt and total polish of: %f"), pow);
}

void UGemAbilities::WaterStream(float pow)
{
	UE_LOG(LogTemp, Warning, TEXT("WaterStream and total polish of: %f"), pow);
}

void UGemAbilities::SmokeSpray(float pow)
{
	UE_LOG(LogTemp, Warning, TEXT("SmokeSpray and total polish of: %f"), pow);
}

UGemAbilities::AbilityPtr* UGemAbilities::FindAbility(int GemCombo)
{
	return abilities.Find(GemCombo);
}

UGemAbilities::UGemAbilities()
{
	//It's a packed 2 bit int. (This technically means we can support up to 16 types before increasing net/cpu load)
	//Next, is order --> Fire, Water, Light, Dark

	//For example: 11 00 10 01 is Dark 3, Light 0, Water 2, Fire 1
	abilities.Add(convertToInteger(1,0,0,0), &UGemAbilities::FireBolt);
	abilities.Add(convertToInteger(0,1,0,0), &UGemAbilities::WaterStream);
	abilities.Add(convertToInteger(1,1,0,0), &UGemAbilities::SmokeSpray);
}
