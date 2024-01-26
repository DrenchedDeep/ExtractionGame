


#include "Quests/QuestController.h"

void AQuestController::InitQuests()
{
	if(QuestDataTable)
	{
		for(auto	Row : QuestDataTable->GetRowMap())
		{
			//	const FQuestInfo* QuestInfo = Cast<FQuestInfo>(Row.Value);
		
			//PlayerQuests.Add(*QuestInfo);
		}
	}
}
