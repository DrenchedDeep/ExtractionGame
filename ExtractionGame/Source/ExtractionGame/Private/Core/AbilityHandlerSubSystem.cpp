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
	if (const TSubclassOf<UGameplayAbility> FoundObject = *ObjectMap.Find(Index))
	{
		return FoundObject;
	}
	// Handle the case where the index is not found
	return nullptr;
}

int UAbilityHandlerSubSystem::ConvertToIntID(int Earth, int Fire, int Shadow, int Water)
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
	ObjectMap.Add(Index, Object);
}
