

#pragma once

#include "CoreMinimal.h"
#include "Gem.h"
#include "GemAbilities.h"

/**
 * 
 */
class Attack
{
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AGem*> Gems; // Not sure why there's a warning here..
	float totalPolish;
	void Recompile();
public:
	Attack();

	void Invoke() const;
	
	AGem* RemoveGem(int id);
	void AddGem(AGem* gem, int slot);
	
	~Attack();
};
