


#include "Core/Other/PlayerSaveData.h"

void UPlayerSaveData::SetSeenCutscene(bool state)
{
	bHasSeenIntroCutscene = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialCrashLand(bool state)
{
	bTutorialCrashLand = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialInventory(bool state)
{
	bTutorialInventory = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}

void UPlayerSaveData::SetTutorialExtraction(bool state)
{
	bTutorialExtraction = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))
}

void UPlayerSaveData::SetTutorialDeath(bool state)
{
	BTutorialDeath = state;
	UGameplayStatics::SaveGameToSlot(this, "Game", 0);
	UE_LOG(LogTemp, Warning, TEXT("Saving data"))

}
