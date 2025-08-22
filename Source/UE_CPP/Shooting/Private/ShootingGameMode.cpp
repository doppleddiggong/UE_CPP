// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "ShootingGameMode.h"
#include "Shared/FComponentHelper.h"
#include "UMainWidget.h"

#define MAINWIDGET_PATH TEXT("/Game/CustomContents/UI/WBP_UI.WBP_UI_C")

AShootingGameMode::AShootingGameMode()
{
	Score = 0;

	MainWidgetFactory = FComponentHelper::LoadClass<UMainWidget>(MAINWIDGET_PATH);
}

void AShootingGameMode::BeginPlay()
{
	Super::BeginPlay();

	MainWidget = CreateWidget<UMainWidget>(GetWorld(), MainWidgetFactory);
	MainWidget->AddToViewport();

	this->SetScore(Score);
}
