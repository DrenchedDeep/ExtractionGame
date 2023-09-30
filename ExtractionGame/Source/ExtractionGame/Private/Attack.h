

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
	TArray<UGem*> Gems; // Not sure why there's a warning here..
	float totalPolish;
	void Recompile();
public:
	Attack();

	void Invoke() const;
	
	UGem* RemoveGem(int id);
	void AddGem(UGem* gem, int slot);
	
	~Attack();
};
