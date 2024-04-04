


#include "ExtractionGameBPLibrary.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


void UExtractionGameBPLibrary::FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject,
	const FVector& InLocation, const float EdgePercent, const FVector2D ViewportCenterLoc, FVector2D& OutScreenPosition,
	float& OutRotationAngleDegrees, bool& bIsOnScreen)
{ 
	bIsOnScreen = false;
OutRotationAngleDegrees = 0.f;
FVector2D ScreenPosition = FVector2D();

if( !GEngine )
{
return;
}

	FIntPoint ScreenSize = GEngine->GameViewport->Viewport->GetSizeXY();
const FVector2D ViewportSize = FVector2D( ScreenSize.X, ScreenSize.Y );
const FVector2D ViewportCenter = FVector2D( ViewportSize.X * ViewportCenterLoc.X, ViewportSize.Y * ViewportCenterLoc.Y );

UWorld* World = GEngine->GetWorldFromContextObject( WorldContextObject, EGetWorldErrorMode::LogAndReturnNull );

if( !World )
{
return;
}

APlayerController* PlayerController = ( WorldContextObject ? UGameplayStatics::GetPlayerController( WorldContextObject, 0 ) : NULL );

if( !PlayerController )
{
return;
}

FVector CameraLoc;
FRotator CameraRot;

PlayerController->GetPlayerViewPoint( CameraLoc, CameraRot );

const FVector CameraToLoc = InLocation - CameraLoc;
FVector Forward = CameraRot.Vector();
FVector Offset = CameraToLoc.GetSafeNormal();

float DotProduct = FVector::DotProduct( Forward, Offset );
bool bLocationIsBehindCamera = ( DotProduct < 0 );

if( bLocationIsBehindCamera )
{
FVector Inverted = CameraToLoc * -1.f;
FVector NewInLocation = CameraLoc + Inverted;

PlayerController->ProjectWorldLocationToScreen( NewInLocation, ScreenPosition );

ScreenPosition.X = ViewportSize.X - ScreenPosition.X;
ScreenPosition.Y = ViewportSize.Y - ScreenPosition.Y;
}
else
{
PlayerController->ProjectWorldLocationToScreen( InLocation, ScreenPosition );
}

// Check to see if it's on screen. If it is, ProjectWorldLocationToScreen is all we need, return it.
if( ScreenPosition.X >= 0.f && ScreenPosition.X <= ViewportSize.X
&& ScreenPosition.Y >= 0.f && ScreenPosition.Y <= ViewportSize.Y && !bLocationIsBehindCamera )
{
OutScreenPosition = ScreenPosition;
bIsOnScreen = true;
return;
}

ScreenPosition -= ViewportCenter;

float AngleRadians = FMath::Atan2( ScreenPosition.Y, ScreenPosition.X );
AngleRadians -= FMath::DegreesToRadians( 90.f );

OutRotationAngleDegrees = FMath::RadiansToDegrees( AngleRadians ) + 180.f;

float Cos = cosf( AngleRadians );
float Sin = -sinf( AngleRadians );

ScreenPosition = FVector2D( ViewportCenter.X + ( Sin * 150.f ), ViewportCenter.Y + Cos * 150.f );

float m = Cos / Sin;

FVector2D ScreenBounds = ViewportCenter * EdgePercent;

if( Cos > 0 )
{
ScreenPosition = FVector2D( ScreenBounds.Y / m, ScreenBounds.Y );
}
else
{
ScreenPosition = FVector2D( -ScreenBounds.Y / m, -ScreenBounds.Y );
}

if( ScreenPosition.X > ScreenBounds.X )
{
ScreenPosition = FVector2D( ScreenBounds.X, ScreenBounds.X * m );
}
else if( ScreenPosition.X < -ScreenBounds.X )
{
ScreenPosition = FVector2D( -ScreenBounds.X, -ScreenBounds.X * m );
}

	ScreenPosition += ViewportCenter;

	OutScreenPosition = ScreenPosition;
}

ERarityType UExtractionGameBPLibrary::GetRarityTypeFromGemPurity(float Purity)
{
	ERarityType Rarity = ERarityType::Common;
	if(Purity >= 75)
	{
		Rarity = ERarityType::Legendary;
	}
	else if(Purity >= 40 && Purity < 75)
	{
		Rarity = ERarityType::Epic;
	}
	else if(Purity >= 25 && Purity < 40)
	{
		Rarity = ERarityType::Rare;
	}
	else if(Purity >= 15 && Purity < 25)
	{
		Rarity = ERarityType::Uncommon;
	}
	else if(Purity >= 0 && Purity < 15)
	{
		Rarity = ERarityType::Common;
	}

	return Rarity;
}

int32 UExtractionGameBPLibrary::GetAdjustedPrice(ERarityType Rarity, int32 BasePrice, float Purity)
{
	int32 AdjustedPrice =  0;

	switch(Rarity)
	{
	case ERarityType::Common:
		AdjustedPrice = 10;
		break;
	case ERarityType::Uncommon:
		AdjustedPrice = 15;
		break;
	case ERarityType::Rare:
		AdjustedPrice = 25;
		break;
	case ERarityType::Epic:
		AdjustedPrice = 75;
		break;
	case ERarityType::Legendary:
		AdjustedPrice = 150;
		break;
	default:
		AdjustedPrice = 100;
		break;
	}

	return AdjustedPrice;
}

