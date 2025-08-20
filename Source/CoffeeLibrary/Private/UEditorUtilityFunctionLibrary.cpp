// Copyright (c) 2025 Doppleddiggong. All rights reserved. Unauthorized copying, modification, or distribution of this file, via any medium is strictly prohibited. Proprietary and confidential.

#include "UEditorUtilityFunctionLibrary.h"
#include "Styling/SlateIconFinder.h"
#include "Styling/AppStyle.h"
#include "UObject/UObjectGlobals.h"

FSlateBrush UEditorUtilityFunctionLibrary::GetClassIconBrush(const UClass* InClass, const bool bSmallIcon)
{
	FSlateBrush Result;

#if WITH_EDITOR
	if (InClass)
	{
		const FSlateIcon Icon = FSlateIconFinder::FindIconForClass(InClass);
		const FSlateBrush* Brush = bSmallIcon ? Icon.GetSmallIcon() : Icon.GetIcon();

		if (Brush)
		{
			Result = *Brush;
			if (Result.ImageSize.IsNearlyZero())
			{
				Result.ImageSize = FVector2D(bSmallIcon ? 16.f : 32.f, bSmallIcon ? 16.f : 32.f);
			}
		}
	}
#endif

	return Result;
}

bool UEditorUtilityFunctionLibrary::RemoveActor(TArray<AActor*>& Array, AActor* Target)
{
	if (!IsValid(Target))
		return false;
	const int32 Idx = Array.IndexOfByKey(Target);
	
	if (Idx == INDEX_NONE)
		return false;
	
	Array.RemoveAt(Idx);
	return true;
}

bool UEditorUtilityFunctionLibrary::MoveActor(TArray<AActor*>& Array, AActor* Target, int32 Delta, int32& OutNewIndex)
{
	OutNewIndex = -1;
	if (!IsValid(Target) || Delta == 0)
		return false;

	const int32 From = Array.IndexOfByKey(Target);
	if (!Array.IsValidIndex(From))
		return false;

	const int32 To = From + Delta;
	if (!Array.IsValidIndex(To))
		return false;

	Array.Swap(From, To);
	OutNewIndex = To;
	return true;
}