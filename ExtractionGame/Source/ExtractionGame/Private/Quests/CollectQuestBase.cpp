


#include "Quests/CollectQuestBase.h"


bool ACollectQuestBase::CheckIfQuestIsComplete()
{
	for(int32 i = 0; i < ItemsNeeded.Num(); i++)
	{
		for(int32 z = 0; z < ItemsCollected.Num(); z++)
		{
			if(ItemsCollected[z].ItemsNeededIndex == i)
			{
				if(ItemsCollected[z].ItemAmount < ItemsNeeded[i].ItemAmount)
				{
					return false;
				}
			}
		}
	}

	return true;
}
