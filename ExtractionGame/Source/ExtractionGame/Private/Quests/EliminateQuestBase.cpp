


#include "Quests/EliminateQuestBase.h"

bool AEliminateQuestBase::CheckIfQuestIsComplete()
{
	if(EliminateCount >= EliminateAmount)
	{
		return true;
	}

	return false;
}
