// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Styling/SlateTypes.h"
#include "UEditorUtilityFunctionLibrary.generated.h"

UCLASS()
class COFFEELIBRARY_API UEditorUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="EUW|Icon", meta=(DisplayName="Get Class Icon Brush (Editor)"))
	static FSlateBrush GetClassIconBrush(const UClass* InClass, const bool bSmallIcon);

	UFUNCTION(BlueprintCallable, Category="EUW|List", meta=(DisplayName="Remove Actor (In-Place)"))
	static bool RemoveActor(UPARAM(ref) TArray<AActor*>& Array, AActor* Target);

	UFUNCTION(BlueprintCallable, Category="EUW|List", meta=(DisplayName="Move Actor (In-Place)"))
	static bool MoveActor(UPARAM(ref) TArray<AActor*>& Array, AActor* Target, int32 Delta, int32& OutNewIndex);
};