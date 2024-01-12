


#include "Objects/ReplicatedItemObject.h"

void UReplicatedItemObject::Destroy()
{
	checkf(GetOwningActor()->HasAuthority(), TEXT("Destroy:: Object does not have authority to destroy itself!"));
	OnDestroyed();
	MarkAsGarbage();
}

void UReplicatedItemObject::OnDestroyed()
{
	
}

bool UReplicatedItemObject::CallRemoteFunction(UFunction* Function, void* Parameters, FOutParmRec* OutParams,
                                               FFrame* Stack)
{
	//return Super::CallRemoteFunction(Function, Parameters, OutParams, Stack);

	check(!HasAnyFlags(RF_ClassDefaultObject))

	AActor* Owner = GetOwningActor();
	UNetDriver* NetDriver = Owner->GetNetDriver();

	if(NetDriver)
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parameters, OutParams, Stack, this);
		return true;
	}

	return false;
}
