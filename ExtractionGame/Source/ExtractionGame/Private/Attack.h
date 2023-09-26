

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
	UGemAbilities::AbilityPtr ptr;
	float totalPolish;
	void Recompile();
public:
	Attack();

	void Invoke() const;
	
	UGem* RemoveGem(int id);
	UGem* AddGem(int id);
	
	~Attack();
};
