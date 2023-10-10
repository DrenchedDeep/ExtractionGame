#include "Core/AbilityHandlerSubSystem.h"

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

TSubclassOf<UGameplayAbility> UAbilityHandlerSubSystem::GetAbilityByIndex(int32 Index)
{
	UE_LOG(LogTemp, Warning, TEXT("Map count: %d, find: %d"),ObjectMap.Num(),Index);

	if(ObjectMap.Num() == 0) return nullptr;
	if(!ObjectMap.Contains(Index))
	{
		UE_LOG(LogTemp, Error, TEXT("INVALID COMBO: %d"), Index)
		return *ObjectMap.Find(0);
	}
	if (const TSubclassOf<UGameplayAbility> FoundObject = *ObjectMap.Find(Index))
	{
		return FoundObject;
	}
	// Handle the case where the index is not found
	return nullptr;
}

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
void UAbilityHandlerSubSystem::AddAbilityToMap(int32 Index, TSubclassOf<UGameplayAbility> Object)
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
