

#pragma once

#include "CoreMinimal.h"
#include "PlayerStand.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameModeBase.generated.h"


UCLASS()
class EXTRACTIONGAME_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<APlayerStand*> PlayerStands;
	
	int32 PartyPlayerCount;
	
public:
	AMainMenuGameModeBase();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	void SetupMemberModel(APlayerState* PlayerState, const FString& Username);

	FName TargetSessionName;

	
	FORCEINLINE void SetTargetSession(const FName SessionName)
	{
		TargetSessionName = SessionName;
	}
	
	FORCEINLINE void ResetTargetSession()
	{
		TargetSessionName = "";
	}
};
