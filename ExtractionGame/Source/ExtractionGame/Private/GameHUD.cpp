


#include "GameHUD.h"

void AGameHUD::CustomNetworkError(FString ErrorReason, FString ErrorDescription)
{
	if(NetworkErrorWidget)
	{
		NetworkErrorWidget->OnNetworkError(ErrorReason, ErrorDescription);
	}
}

void AGameHUD::CreateNetworkErrorWidget()
{
	NetworkErrorWidget = CreateWidget<UNetworkErrorWidget>(GetWorld(), NetworkErrorWidgetSubclass);
	if(NetworkErrorWidget)
	{
		NetworkErrorWidget->AddToViewport();
	}
}