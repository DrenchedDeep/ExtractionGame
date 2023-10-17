


#include "Gem.h"

#include "Net/UnrealNetwork.h"

AGem::AGem()
{
	bReplicates = true;
	bAlwaysRelevant = true;
}

float AGem::GetPolish() const
{
	return Polish;
}

EGemType AGem::GetGemType() const
{
	return GemType;
}

void AGem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGem, GemType);
	DOREPLIFETIME(AGem, Polish);

}
