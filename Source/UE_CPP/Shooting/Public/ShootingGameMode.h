// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UMainWidget.h"
#include "ShootingGameMode.generated.h"

UCLASS()
class UE_CPP_API AShootingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AShootingGameMode();

	virtual void BeginPlay() override;

	__declspec(property(get=GetScore, put=SetScore)) int32 SCORE;
	FORCEINLINE int32 GetScore() const
	{
		return this->Score;
	}
	
	FORCEINLINE void SetScore(int32 NewScore)
	{
		this->Score = NewScore;
		MainWidget->UpdateTextScore(Score);
		UE_LOG(LogTemp, Warning, TEXT("%d"), Score);
	}
		
	FORCEINLINE void AddScore(int32 AddScore)
	{
		this->Score += AddScore;
		MainWidget->UpdateTextScore(Score);
		UE_LOG(LogTemp, Warning, TEXT("%d"), Score);
	}

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> MainWidgetFactory;
	
	UPROPERTY()
	class UMainWidget* MainWidget; 

private:
	int32 Score;
};
