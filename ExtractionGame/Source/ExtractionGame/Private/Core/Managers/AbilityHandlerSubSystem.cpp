#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "Abilities/GameplayAbility.h"

void UAbilityHandlerSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	ObjectMap = {};
	UE_LOG(LogTemp, Warning, TEXT("Initialized SubSys"));

}

void UAbilityHandlerSubSystem::Deinitialize()
{
	Super::Deinitialize();
}

FAbilityStruct UAbilityHandlerSubSystem::GetAbilityByIndex(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Map count: %d, find: %d"),ObjectMap.Num(),Index);

	if(ObjectMap.Num() == 0) return FAbilityStruct();
	if(!ObjectMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("INVALID COMBO: %d"), Index)
		return *ObjectMap.Find(0);
	}
	if (const FAbilityStruct* FoundObject = ObjectMap.Find(Index))
	{
		return *FoundObject;
	}
	// Handle the case where the index is not found
	return FAbilityStruct();
}

TSubclassOf<UGameplayEffect> UAbilityHandlerSubSystem::GetEffectByIndex(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Map count: %d, find: %d"),EffectMap.Num(),Index);

	if(EffectMap.Num() == 0) return nullptr;
	if(!EffectMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("INVALID COMBO: %d"), Index)
		return *EffectMap.Find(0);
	}
	if (const TSubclassOf<UGameplayEffect> FoundObject = *EffectMap.Find(Index))
	{
		return FoundObject;
	}
	// Handle the case where the index is not found
	return nullptr;
}

/*
UTexture2D* UAbilityHandlerSubSystem::GetGemSprite(const AGem* gem)
{

	int Index;
	const float p = gem->GetPolish();
	if(p > 150) Index = 3;
	else if(p > 75) Index = 2;
	else Index = 1;

	Index = Index << static_cast<int>(gem->GetGemType());
	
	UE_LOG(LogTemp, Warning, TEXT("Map count: %d, find: %d"),ObjectMap.Num(),Index);

	if(IconsMap.Num() == 0) return nullptr;
	if(!IconsMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("INVALID COMBO: %d"), Index)
		return *IconsMap.Find(0);
	}
	return *IconsMap.Find(Index);
}

void UAbilityHandlerSubSystem::AddGemSpriteToMap(int32 Index, UTexture2D* Object)
{
#if UE_EDITOR
	if(IconsMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Key collision: %d"),  Index);
		return;
	}
#endif
	IconsMap.Add(Index, Object);
}
*/
int32 UAbilityHandlerSubSystem::ConvertToIntID(int32 Earth, int32 Fire, int32 Shadow, int32 Water)
{
	// Assuming each parameter can have values 1, 2, or 3
	int32 Result = (Earth & 3); //10
	Result <<= 2; //1000

	Result |= (Fire & 3); //1001
	Result <<= 2; //100100
	//
	Result |= (Shadow & 3);//100111
	Result <<= 2;//10011100

	Result |= (Water & 3);//10011101
	
	return Result;
}
void UAbilityHandlerSubSystem::AddAbilityToMap(int32 Index,FAbilityStruct Object)
{
#if UE_EDITOR
	if(ObjectMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Key collision: %d"),  Index);
		return;
	}
#endif
	ObjectMap.Add(Index, Object);
}

void UAbilityHandlerSubSystem::AddEffectToMap(int32 Index, TSubclassOf<UGameplayEffect> Object)
{
#if UE_EDITOR
	if(EffectMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("Key collision: %d"),  Index);
		return;
	}
#endif
	EffectMap.Add(Index, Object);
}

bool UAbilityHandlerSubSystem::ContainsAbility(int index) const
{
	return ObjectMap.Contains(index);
}

bool UAbilityHandlerSubSystem::ContainsEffect(int index) const
{
	return EffectMap.Contains(index);
}
