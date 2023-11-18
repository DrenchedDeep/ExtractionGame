


#include "SebDoor.h"

#include "Components/SphereComponent.h"
#include "ExtractionGame/ExtractionGameCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASebDoor::ASebDoor()
{
 	//We are not a ticking actor, our child is just handling static collisions.
	PrimaryActorTick.bCanEverTick = false;
	
	//Try to find the object, else let's create one.
	TriggerComponent = FindComponentByClass<UPrimitiveComponent>();
	if(!TriggerComponent)
	{
		//Create a new object
		USphereComponent *c = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerRange"));
		// Give default size
		c->InitSphereRadius(500);

		//Only query collisions, and overlap pawns
		c->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		c->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
		
		TriggerComponent = c;
	}
	
	//Add a trigger component, and bind our events to their events.
	TriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ASebDoor::BeginOverlap);
	TriggerComponent->OnComponentEndOverlap.AddDynamic(this, &ASebDoor::EndOverlap);
}

void ASebDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//Mark the open state as replicated, so that if someone joins late this property is maintained.
	DOREPLIFETIME(ASebDoor, bIsOpen)
}

void ASebDoor::BeginPlay()
{
	Super::BeginPlay();
	//On Begin play, update the doors state.
	if(bIsOpen) OpenDoor(); // If open, invoke the function
	else CloseDoor(); // else close the door.
}


//RPC implementation on all clients
void ASebDoor::SetDoorState_Implementation(bool state)
{
	bIsOpen = state; // Set the doors new state
	if(state) OpenDoor(); // If open, invoke the function
	else CloseDoor(); // else close the door.
} 

void ASebDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//If we are the server, AND the door is not already open AND the character interacting with us is a player. THEN do stuff.
	if(!HasAuthority() || bIsOpen || !OtherActor->IsA(AExtractionGameCharacter::StaticClass())) return;
	SetDoorState(true);
	
}

void ASebDoor::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//If we are the server, AND the door is not already closed AND the character interacting with us is a player. THEN do stuff.
	if(!HasAuthority() || !bIsOpen || !OtherActor->IsA(AExtractionGameCharacter::StaticClass())) return;

	//Cache then number of actors
	TArray<AActor*> overlapped;
	
	//Only look for players
	const TSubclassOf<AExtractionGameCharacter> filter = AExtractionGameCharacter::StaticClass();
	
	//Check if anyone is overlapping still
	TriggerComponent->GetOverlappingActors(overlapped, filter);
	
	//If not, then close.
	if(overlapped.Num() == 0)SetDoorState(false);
}



