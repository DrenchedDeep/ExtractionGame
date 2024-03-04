


#include "MainMenuSpaceShip.h"

void AMainMenuSpaceShip::OnRep_Owner()
{
	Super::OnRep_Owner();

	UE_LOG(LogTemp, Warning, TEXT("OnRep_Owner"));
	SetupSpaceship();
}

void AMainMenuSpaceShip::SetOwner(AActor* NewOwner)
{
	Super::SetOwner(NewOwner);

	SetupSpaceship();
}
