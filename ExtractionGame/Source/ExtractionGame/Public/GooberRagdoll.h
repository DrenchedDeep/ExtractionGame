
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GooberRagdoll.generated.h"

UCLASS()
class EXTRACTIONGAME_API AGooberRagdoll : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGooberRagdoll();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
