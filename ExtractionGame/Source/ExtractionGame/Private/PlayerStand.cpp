


#include "PlayerStand.h"

#include "Net/UnrealNetwork.h"

APlayerStand::APlayerStand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
}

void APlayerStand::OnRep_IsOccupied()
{
	ToggleCharacterVisibility(bIsOccupied);
}

void APlayerStand::OnRep_Username()
{
	UpdateUsernameText(Username);
}

void APlayerStand::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerStand, bIsOccupied);
	DOREPLIFETIME(APlayerStand, Username);
}

