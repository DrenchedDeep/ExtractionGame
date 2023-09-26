


#include "Attack.h"

Attack::Attack()
{
}

void Attack::Invoke() const
{
	ptr(totalPolish);
}

void Attack::Recompile()
{
	ptr = UGemAbilities::FindAbility(1);
}

UGem* Attack::RemoveGem(int id)
{
	
}

UGem* Attack::AddGem(int id)
{
	
}

Attack::~Attack()
{
}
