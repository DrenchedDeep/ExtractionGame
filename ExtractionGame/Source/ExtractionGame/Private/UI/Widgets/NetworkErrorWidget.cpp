


#include "UI/Widgets/NetworkErrorWidget.h"

#include "Kismet/GameplayStatics.h"

void UNetworkErrorWidget::CloseMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::OpenLevel(GetWorld(), "LVL_Startup");
}

void UNetworkErrorWidget::OnNetworkError(FString ErrorHeader, FString ErrorDescription)
{
	if(ErrorHeaderText && ErrorDescriptionText)
	{
		ErrorHeaderText->SetText(FText::FromString(ErrorHeader));
		ErrorDescriptionText->SetText(FText::FromString(ErrorDescription));
		SetVisibility(ESlateVisibility::Visible);
	}
}
