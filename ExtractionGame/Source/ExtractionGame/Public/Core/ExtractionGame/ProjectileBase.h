

#pragma once

#include "CoreMinimal.h"
#include "Core/ExtractionGame/PooledObject.h"
#include "ProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class EXTRACTIONGAME_API AProjectileBase : public APooledObject
{
	GENERATED_BODY()


public:
	virtual void OnRep_Owner() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SafeBeginPlay();

protected:
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override; 
};
