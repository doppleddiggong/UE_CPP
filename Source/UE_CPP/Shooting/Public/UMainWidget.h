// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UMainWidget.generated.h"

UCLASS()
class UE_CPP_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void UpdateTextScore(const int Score) const
	{
		FNumberFormattingOptions Format;
		Format.SetUseGrouping(true);  // 3자리마다 쉼표
		Format.SetMinimumIntegralDigits(1);
		TextScore->SetText(FText::AsNumber(Score, &Format));
		
		// TextScore->SetText( FText::AsNumber(Score) );
	}
	
	UPROPERTY( meta=(BindWidget) )
	class UTextBlock* TextScore;
};
