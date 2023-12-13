#include "Core/ExtractionGame/ProjectileBase.h"

void AProjectileBase::OnRep_Owner()
{
	Super::OnRep_Owner();

	SafeBeginPlay();
}

bool AProjectileBase::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget,
	const FVector& SrcLocation) const
{
	if(GetOwner() && GetOwner()->GetOwner())
	{
		return RealViewer != GetOwner()->GetOwner();
	}
	
	return Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

}
