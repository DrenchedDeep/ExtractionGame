

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPGem.generated.h"

UCLASS()
class EXTRACTIONGAME_API ATPGem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPGem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
