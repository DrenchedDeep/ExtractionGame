


#include "Managers/CommandHandlerSubsystem.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"
#include "Core/Managers/AbilityHandlerSubSystem.h"
#include "Engine/DamageEvents.h"
#include "Items/ItemActor.h"
#include "Net/UnrealNetwork.h"

void UCommandHandlerSubsystem::SetTimeOfDay(float time) const
{
	//12.5
	if(!GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
		return;
	}
	const int val = static_cast<int>(time);
	const float fract = time - val; // 12.5 - 12 == 0.5
	const float t =  static_cast<int>(val) % 24 + fract; // 12.5/24
	OnTimeOfDayChanged.Broadcast(t);
}

void UCommandHandlerSubsystem::SpawnActorOnSelf(FName ClassName) const
{
	if(!GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
		return;
	}
	//Get the world
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UNABLE TO GET WORLD, COULD NOT SPAWN ACTOR"))
		return;
	}

	// Get the pawn or spectator of the local player controller
	const APawn* PlayerPawn = GetGameInstance()->GetFirstLocalPlayerController()->GetPawnOrSpectator();

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PAWN OR SPECTATOR FOUND, COULD NOT SPAWN ACTOR"))
		return;
	}
	const AExtractionGameCharacter* ce = Cast<AExtractionGameCharacter>(PlayerPawn);
	// Get the forward vector of the player's view
	FVector SpawnLocation ;
	if(ce && FVector::Distance(ce->GetActorLocation(),ce->GazeLocation) < 5000) SpawnLocation =  ce->GazeLocation + FVector(0,0,200); // Adjust the distance as needed
	else SpawnLocation= PlayerPawn->GetActorLocation() + PlayerPawn->GetActorForwardVector() * 200.0f;
	// Get the class of the actor based on the FName
	const UObject* ActorClass = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), nullptr, *ClassName.ToString()));
	const UBlueprint* GeneratedBP = Cast<UBlueprint>(ActorClass);
	if (!GeneratedBP)
	{
		UE_LOG(LogTemp, Error, TEXT("ACTOR CLASS NOT FOUND, COULD NOT SPAWN ACTOR"))
		return;
	}

	// Check if the class is derived from AActor and is a Blueprint class
	if (ActorClass->StaticClass()->IsChildOf(AActor::StaticClass()))
	{
		// Spawn the actor in front of the player
		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnLocation, FRotator::ZeroRotator);
		UE_LOG(LogTemp, Error, TEXT("Spawned actor at : %s"), *SpawnLocation.ToString())
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACTOR CLASS IS NOT A BLUEPRINT CLASS: %s"), *ClassName.ToString());
	}
}

void UCommandHandlerSubsystem::SpawnGemOnSelf(int type, float polish) const
{

	if(!GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
		return;
	}
	
	//Get the world
	UWorld* World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UNABLE TO GET WORLD, COULD NOT SPAWN ACTOR"))
		return;
	}

	// Get the pawn or spectator of the local player controller
	const APawn* PlayerPawn = GetGameInstance()->GetFirstLocalPlayerController()->GetPawnOrSpectator();

	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PAWN OR SPECTATOR FOUND, COULD NOT SPAWN ACTOR"))
		return;
	}
	const AExtractionGameCharacter* ce = Cast<AExtractionGameCharacter>(PlayerPawn);
	// Get the forward vector of the player's view
	FVector SpawnLocation ;
	if(ce && FVector::Distance(ce->GetActorLocation(),ce->GazeLocation) < 5000) SpawnLocation =  ce->GazeLocation + FVector(0,0,200); // Adjust the distance as needed
	else SpawnLocation= PlayerPawn->GetActorLocation() + PlayerPawn->GetActorForwardVector() * 200.0f;


	const UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Game/Inventory/Items/BP_Item.BP_Item'")));
	const UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if(!GeneratedBP)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn gem Spawned gem at: %s -- > /Script/Engine.Blueprint'/Game/Inventory/Items/BP_Item.BP_Item'"), *SpawnLocation.ToString())
	}

	
	// Spawn the actor in front of the player
	if(AItemActor * gem = World->SpawnActor<AItemActor>(GeneratedBP->GeneratedClass, SpawnLocation, FRotator::ZeroRotator))
	{
		UE_LOG(LogTemp, Error, TEXT("Spawned gem at : %s"), *SpawnLocation.ToString())
		switch(type)
		{
		case 0:
			gem->RowName = "WaterGem0-25%";
			break;
		case 1 :
			gem->RowName = "FireGem0-25%";
			break;
		case 2:
			gem->RowName = "ShadowGem0-25%";
			break;
		case 3:
		gem->RowName = "WaterGem0-25%";
			break;
		default:
			gem->RowName = "WaterGem0-25%";
			break;
		}
		
		gem->DefaultPolish = polish;
	}
	
}


void UCommandHandlerSubsystem::TPAllToMe(bool avoidObstacles,float spawnRadius ) const
{
	const APlayerController* controller = GetGameInstance()->GetFirstLocalPlayerController();
	if(!controller->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
		return;
	}
	if(const APawn* spawner = controller->GetPawnOrSpectator()) TeleportAllPlayers(controller, spawner->GetActorLocation(), spawnRadius, avoidObstacles);
	
	//else
	
	
}

void UCommandHandlerSubsystem::TPAllToLocation(float x, float y, float z, bool avoidObstacles, float spawnRadius) const
{
	const APlayerController* controller = GetGameInstance()->GetFirstLocalPlayerController();
	if(!controller->HasAuthority())
	{
		UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
		return;
	}
	TeleportAllPlayers(nullptr, FVector(x,y,z), spawnRadius, avoidObstacles);
}

void UCommandHandlerSubsystem::TeleportAllPlayers(const AController *caller, const FVector& location, float radius, bool avoidObstacles) const
{
	UWorld* World = GetWorld();
    
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UNABLE TO GET WORLD, COULD NOT TELEPORT"))
		return;
	}
	
	for (FConstPlayerControllerIterator It = World->GetPlayerControllerIterator(); It; ++It)
	{
		const APlayerController* PlayerController = It->Get();
		const APawn* pawn = PlayerController->GetPawn();
		if (PlayerController && PlayerController != caller && pawn)
		{
			if (ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn()))
			{
				// Generate random angle in radians
				float RandomAngle = FMath::FRandRange(0.0f, 2.0f * PI);

				// Calculate random offset within the specified radius
				FVector RandomOffset = FVector(FMath::Cos(RandomAngle), FMath::Sin(RandomAngle), 0.0f) * radius;

				// Calculate the target location with random offset
				FVector NewTargetLocation = location + RandomOffset;

				// If AvoidObstacles is true, perform collision check
				if (avoidObstacles)
				{
					FHitResult HitResult;
					FCollisionQueryParams CollisionParams;
					CollisionParams.AddIgnoredActor(PlayerCharacter);

					// Perform a line trace to check for obstacles
					FVector TraceStart = NewTargetLocation;
					FVector TraceEnd = NewTargetLocation - FVector(0.0f, 0.0f, 10000.0f); // Trace downwards

					if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
					{
						// Adjust the target location based on the hit result
						NewTargetLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, PlayerCharacter->GetSimpleCollisionRadius());
					}
				}
				UE_LOG(LogTemp,Warning,TEXT("Setting actor position: %s"), *NewTargetLocation.ToString())
				// Teleport the player to the adjusted target location
				PlayerCharacter->SetActorLocation(NewTargetLocation);
			}
		}
	}
}

void UCommandHandlerSubsystem::EnableDebugInfo() const
{
	UE_LOG(LogTemp, Warning, TEXT("This function need more work, but here's your position: %s"), *GetGameInstance()->GetFirstLocalPlayerController()->GetPawnOrSpectator()->GetActorLocation().ToString())
}

void UCommandHandlerSubsystem::KillSelf() 
{
	//Only the server is actually able to do this... Only the server can distribute damage?
	if (ACharacter* Character = GetGameInstance()->GetFirstLocalPlayerController()->GetCharacter())
	{
		const UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), nullptr, TEXT("/Script/Engine.Blueprint'/Engine/EngineDamageTypes/DmgTypeBP_Environmental.DmgTypeBP_Environmental'")));
		const UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if(!GeneratedBP)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn gem Spawned gem at: %s -- > /Script/Engine.Blueprint'/Engine/EngineDamageTypes/DmgTypeBP_Environmental.DmgTypeBP_Environmental'"))
		}

		const UDamageType* DamageType = Cast<UDamageType>(GeneratedBP->GeneratedClass); // Replace UYourDamageType with the actual damage type class you want to use

		
		Character->ReceiveAnyDamage(100000, DamageType, GetGameInstance()->GetFirstLocalPlayerController(), GetGameInstance()->GetFirstLocalPlayerController());
		UE_LOG(LogTemp,Warning,TEXT("KIlling self: %s"), *Character->GetName())
		KillSelfServer(GetGameInstance()->GetFirstLocalPlayerController());
	}
}

void UCommandHandlerSubsystem::SetAllManaRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetEarthRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetFireRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetWaterRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::SetShadowRegenSpeed(float val)
{
}

void UCommandHandlerSubsystem::ChangeKnockBackMultiplier(float amt)
{
	if( GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority()) KnockBackMultiplier = amt;
	else UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
	
}

void UCommandHandlerSubsystem::SetDamageMultiplier(float val)
{
	if( GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority()) DamageMultiplier = val;
	else UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
}

void UCommandHandlerSubsystem::SetAttackCooldownMultiplier(float val)
{
	if( GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority()) AttackCooldown = val;
	else UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
}

void UCommandHandlerSubsystem::SetCostMultiplier(float val)
{
	if( GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority()) DamageMultiplier = val;
	else UE_LOG(LogTemp,Warning,TEXT("YOU DO NOT HAVE AUTHORITY TO DO THIS"))
}

void UCommandHandlerSubsystem::SetAbilityDamage(int E, int F, int S, int W, float val)
{
}

void UCommandHandlerSubsystem::SetAbilityFireRate(int E, int F, int S, int W, float val)
{
}

void UCommandHandlerSubsystem::SetAbilityCost(int E, int F, int S, int W, float val) const
{
	const int32 index = AbilitiesSubSystem->ConvertToIntID(E,F,S,W);

	
}

void UCommandHandlerSubsystem::GiveEssence(float amount) const
{
	if(AExtractionGameCharacter* character = Cast<AExtractionGameCharacter>(GetGameInstance()->GetFirstLocalPlayerController()->GetCharacter()))
	{
		character->AddEssence(amount);
	}
}

void UCommandHandlerSubsystem::ClearSaveData() const
{
	UGameplayStatics::DeleteGameInSlot("Game", 0);

	if(UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance()))
	{
		GameInstance->bLoadedInventoryOnStart = false;

		if(GameInstance->PlayerSessionData.bIsValid)
		{
			GameInstance->PlayerSessionData.GemItems.Reset();
			GameInstance->PlayerSessionData.StashItems.Reset();
			GameInstance->PlayerSessionData.PlayerItems.Reset();

		}
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("LVL_Entry"));
}

void UCommandHandlerSubsystem::KillSelfServer_Implementation(APlayerController* target)
{
	target->GetCharacter()->TakeDamage(100000, FDamageEvent(), target, target);
}

void UCommandHandlerSubsystem::SetAbilityCostServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx).GameplayAbilityClass);

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityCostServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}

void UCommandHandlerSubsystem::SetAbilityFireRateServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx).GameplayAbilityClass);

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityFireRateServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}

void UCommandHandlerSubsystem::SetAbilityDamageServer_Implementation(int idx, float val)
{
	//Modify the ability cost...
	const UGameplayAbility* ability = Cast<UGameplayAbility>(AbilitiesSubSystem->GetAbilityByIndex(idx).GameplayAbilityClass);

	FGameplayModifierInfo mod = ability->GetCostGameplayEffect()->Modifiers[0];

	mod.ModifierMagnitude = FGameplayEffectModifierMagnitude(FScalableFloat(-val));
}

bool UCommandHandlerSubsystem::SetAbilityDamageServer_Validate(int idx, float val)
{
	return GetGameInstance()->GetFirstLocalPlayerController()->HasAuthority() && AbilitiesSubSystem && AbilitiesSubSystem->ContainsAbility(idx);
}


void UCommandHandlerSubsystem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCommandHandlerSubsystem, KnockBackMultiplier)
	DOREPLIFETIME(UCommandHandlerSubsystem, DamageMultiplier)
	DOREPLIFETIME(UCommandHandlerSubsystem, AttackCooldown)
}

void UCommandHandlerSubsystem::Init(UAbilityHandlerSubSystem* abilitySys)
{
	AbilitiesSubSystem = abilitySys;
}

