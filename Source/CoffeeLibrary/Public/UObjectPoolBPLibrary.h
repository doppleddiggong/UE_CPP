// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObjectPoolBPLibrary.generated.h"

class UObjectPoolManager;

UCLASS()
class COFFEELIBRARY_API UObjectPoolBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Pool", meta = (WorldContext = "WorldContextObject"))
	static UObjectPoolManager* Get(UObject* WorldContext)
	{
		if (!WorldContext)
			return nullptr;
		
		if (auto* GI = WorldContext->GetWorld()->GetGameInstance())
			return GI->GetSubsystem<UObjectPoolManager>();
		return nullptr;
	}
};