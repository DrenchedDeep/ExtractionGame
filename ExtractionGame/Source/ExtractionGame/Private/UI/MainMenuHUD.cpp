#include "UI/MainMenuHUD.h"

#include "Core/ExtractionGame/ExtractionGameInstance.h"

void AMainMenuHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetSubclass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	
	RaidOverWidget = CreateWidget<URaidOverWidget>(GetWorld(), RaidOverWidgetSubclass);
	RaidOverWidget->AddToViewport();
	RaidOverWidget->SetVisibility(ESlateVisibility::Hidden);
	
	CreateNetworkErrorWidget();
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	UExtractionGameInstance* GameInstance = Cast<UExtractionGameInstance>(GetGameInstance());

	if(GameInstance->PlayerRaidResult.bIsValid)
	{
		RaidOverWidget->InitBP(GameInstance->PlayerRaidResult.bSurvived, GameInstance->PlayerRaidResult.PlayTime);
		MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
		RaidOverWidget->SetVisibility(ESlateVisibility::Visible);
		
		const FPlayerRaidResult TempRaidResult(false,false, 0.0f, 0);
		GameInstance->PlayerRaidResult = TempRaidResult;
	}
}
