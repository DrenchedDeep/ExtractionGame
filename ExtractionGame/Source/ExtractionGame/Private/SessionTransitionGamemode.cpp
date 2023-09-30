


#include "SessionTransitionGamemode.h"

void ASessionTransitionGamemode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerCount++;

	if(PlayerCount >= 2)
	{
		GetWorld()->ServerTravel("SessionMap?listen");
	}
}
